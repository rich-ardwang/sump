#include <iostream>
#include <cstring>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>
#include <vector>
#include <sstream>
#include "lava_utils_api.h"
#include "lava_proto_api.h"
#include "lava_box_api.h"
#include "lava_sema.h"
#include "timer.h"
#include "user_args.h"
#include "common.h"
#include "tokeninfo.h"

#define qb_clients_mock_module         "qb_clients_mock"
using namespace lava::base;
using namespace lava::utils;
using namespace lava::proto;
using namespace lava::comm;
using namespace lava::box;

extern std::atomic<bool> g_thread_exit_flag;
#ifdef LAVA_WIN
#include <windows.h>
#include <stdio.h>
BOOL WINAPI my_handler(DWORD fdwCtrlType);
#else
#include <signal.h>
void my_handler(int s);
#endif

const int CEDA_FID_TOPIC = 3;
const int CEDA_FID_SDN_MSG = 3;
const int SDN_FID_START_SEQ_ID = 4;
const int SDN_FID_END_SEQ_ID = 5;
const int SDN_FID_TOPIC = 7;
const int SDN_FID_REQUEST_MAX_ROW = 11;

class client_impl {
    friend class event_listener;
    friend class message_listener;

    class event_listener : public i_event_listener {
    public:
        event_listener() : m_login_success(false) {}

        bool wait_login() {
            std::mutex dumb_mutex;
            std::unique_lock<std::mutex> lock_guard(dumb_mutex);
            return m_cond.wait_for(lock_guard, std::chrono::seconds(40), [this](){ return m_login_success; });
        }

    private:
        virtual void on_event(int32_t event_code, void* event_data) {
            if (event_code == event_login_success) {
                log_info(qb_clients_mock_module, "receive login sucess.");
                m_login_success = true;
                m_cond.notify_one();
            }
        }

        virtual void on_exception(int32_t error_code, const char* what) {
            log_debug(qb_clients_mock_module, "exception happened: erro_code = %d, desc = %s", error_code, what);
        }

    private:
        std::condition_variable     m_cond;
        volatile bool               m_login_success;
        i_object_impl(std::atomic<int32_t>)
    };

    class message_listener : public i_message_listener {
    public:
        message_listener(class client_impl* impl) : m_parent(impl) {}

    private:
        virtual void on_message(__lv_in i_message* msg) {
            if (!m_parent->m_user_args.test_mode)
                ++m_parent->m_replys;
            else {
                if (msg) {
                    ++m_parent->m_replys;

                    i_message_body *body = msg->get_message_body();
                    if (body) {
                        int ret_code = body->get_int(4);
                        std::string ret_msg = body->get_string(5);
                        std::string ret_body = body->get_string(6);

                        //parse and transform to dds, gtw, http
                        trans_token_info tk_info;
                        memset(&tk_info, 0, sizeof(trans_token_info));
                        std::vector<std::string> vec_data;
                        iam::login_server::server_utils::split(ret_body, ',', vec_data);
                        int idx = 0;
                        for (auto &it : vec_data) {
                            switch (idx) {
                            case 0:
                                memcpy(&tk_info.u_name, it.c_str(), it.length() + 1);
                                break;
                            case 2:
                                memcpy(&tk_info.token, it.c_str(), it.length() + 1);
                                break;
                            default:
                                break;
                            }
                            idx++;
                        }
                        send_to_dds(tk_info);
                        send_to_gtw(tk_info);
                        //send_to_http(tk_info);
                        log_debug(qb_clients_mock_module, "async:on_message:%d, msg_code:%d, ret_msg:%s, ret_body:%s",
                            m_parent->m_replys, ret_code, ret_msg.c_str(), ret_body.c_str());
                    }
                }
            }
        }

        virtual void on_reply(__lv_in i_message* msg, __lv_in const char* topic) {}

    private:
        class client_impl* m_parent;
        i_object_impl(std::atomic<int32_t>)
    };

public:
    client_impl(user_args_type user_args)
        : m_user_args(user_args), m_sub_replys(0), m_replys(0), m_requests(0), m_is_start(false), m_interval(2) {}

    void start_work() {
        for (int i = 0; i < m_user_args.work_threads; i++)
            m_workers.push_back(std::make_shared<std::thread>(std::bind(&client_impl::work, this)));

        // notify all threads to start working.
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        {
            std::unique_lock<std::mutex> lock_guard(m_dumb);
            m_is_start = true;
            m_switch.notify_all();
        }
        log_info(qb_clients_mock_module, "notify_all start");
        if (!m_user_args.test_mode)
            m_timer.start(m_interval * 1000, std::bind(&client_impl::timer_request_cb, this));
    }

    void stop_work() {
        // notify all threads to stop working.
        if (!m_user_args.test_mode)
            m_timer.expire();
        {
            std::unique_lock<std::mutex> lock_guard(m_dumb);
            m_is_start = false;
            m_switch.notify_all();
        }
        log_info(qb_clients_mock_module, "notify_all stop");

        for (int i = 0; i < m_user_args.work_threads; i++)
            m_workers[i]->join();
    }

private:
    void work() {
        log_info(qb_clients_mock_module, "client work thread.");
        test_user_login_req();
    }

    void timer_sub_cb() {
        uint32_t sub_replys = m_sub_replys;
        m_sub_replys = 0;
        log_info(qb_clients_mock_module, "publish messages, average = %d.", sub_replys / m_interval);
    }

    void timer_request_cb() {
        uint32_t replys = m_replys;
        uint32_t requests = m_requests;
        m_replys = 0;
        m_requests = 0;
        log_info(qb_clients_mock_module, "replys average = %d; requests average = %d.", replys / m_interval, requests / m_interval);
    }

    void test_user_login_req() {
        {
            // wait for starting command
            std::unique_lock<std::mutex> lock_guard(m_dumb);
            m_switch.wait(lock_guard, [this](){ return m_is_start; });
        }
        log_info(qb_clients_mock_module, "test_user_login_req in %d.", m_is_start);

        client_login_info info;
        info.m_host = m_user_args.server_addr.c_str();	// "172.16.97.114";"172.16.97.115";
        info.m_port = m_user_args.port.c_str();			// "10020";"10012";
        info.m_username = "loginsrv.lava_server";
        info.m_password = "123.com";
        info.m_use_cluster_mode = m_user_args.use_cluster;
        client_cluster_node_info cluster_info;
        cluster_info.m_cluster_type = cluster_zookeeper;
        cluster_info.m_cluster_server_name = m_user_args.name.c_str();
        cluster_info.m_cluster_nodes_info = m_user_args.cluster_addr.c_str();
        info.m_cluster_info = &cluster_info;
        i_auto_ptr<i_proxy> proxy = create_proxy("test_user_login_req", &info);
        proxy->set_auto_reconnect(true, 3);

        event_listener* event_obj = new event_listener();
        i_auto_ptr<i_event_listener> i_ptr(event_obj);
        proxy->interest_event(i_ptr.get());
        if (false == event_obj->wait_login()) {
            proxy->cancel_event(i_ptr.get());
            return;
        }

        //
        // async-request: firstly you should call as below
        // before sending request.
        //
        i_auto_ptr<i_message_listener> i_ptr2(new message_listener(this));
        uint32_t cookie;
        proxy->advise(i_ptr2.get(), cookie);

        i_message* msg = nullptr;
        int i = 0;
        timer t;
        t.async_wait(60*60*24*7 * 1000, std::bind(&client_impl::stop_post, this));
        start_post();
        while (true) {
            if (get_post_signal())
                break;

            if (m_requests > m_user_args.req_speed)
                continue;

            msg = create_message(i++);
            uint32_t ret = proxy->request_asyn(msg, cookie);
            if (0 == ret) {
                msg->dereference();
                continue;
            }
            ++m_requests;

            // wait for stopping command
            if (false == m_is_start)
                break;

            if (99 == i)
                i = 0;

            if (m_user_args.test_mode)
                std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        proxy->unadvise(cookie);
        proxy->cancel_event(i_ptr.get());
    }

    i_message* create_message(int uid) {
        i_message* msg = get_message(MSG_TYPE_DATA_REQ);
        msg->get_message_body()->add_int(4, 1);
        msg->get_message_body()->add_int(5, 1);
        std::string in_value = "id000";
        in_value += std::to_string(uid);
        in_value += ",";
        in_value += std::to_string(uid) + ".com";
        msg->get_message_body()->add_string(6, in_value.c_str());
        return msg;
    }

    void start_post() {
        m_post_signal.store(false);
    }

    void stop_post() {
        m_post_signal.store(true);
    }

    bool get_post_signal() {
        return m_post_signal;
    }

private:
    user_args_type                              m_user_args;
    std::vector< std::shared_ptr<std::thread> > m_workers;

    std::atomic<uint32_t>                       m_sub_replys;
    std::atomic<uint32_t>                       m_replys;
    std::atomic<uint32_t>                       m_requests;

    std::mutex                                  m_dumb;
    std::condition_variable                     m_switch;
    volatile bool                               m_is_start;

    timer                                       m_timer;
    uint32_t                                    m_interval;
    std::atomic<bool>                           m_post_signal;
    i_object_impl(std::atomic<int32_t>)
};

void mock_qb_clients(user_args_type& user_args) {
    log_info(qb_clients_mock_module, "as lava client: io-thread = %d, worker-thread = %d", user_args.io_threads, user_args.work_threads);
    init_environment(user_args.io_threads, 0);

#ifdef LAVA_WIN
    SetConsoleCtrlHandler(my_handler, TRUE);
#else
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
#endif
    g_thread_exit_flag.store(false);

    std::shared_ptr<client_impl> c = std::make_shared<client_impl>(user_args);
    c->start_work();

    std::cout << "\nPress enter key to stop working..." << std::endl;
    std::cin.get();

    c->stop_work();
    c = nullptr;

    clear_environment();
}