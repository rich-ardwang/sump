#include <cstring>
#include <assert.h>
#include "singleton.h"
#include "lava_server.h"
#include "lava_process.h"

//need extern using out of this file
iam::login_server::i_lava_server *get_lava_server() {
    return singleton_lazy<iam::login_server::lava_server, iam::login_server::i_lava_server>::instance();
}

void release_lava_server() {
    singleton_lazy<iam::login_server::lava_server, iam::login_server::i_lava_server>::release();
}

extern iam::login_server::i_lava_process *get_lava_process();


namespace iam {
namespace login_server {

lava_server::lava_server() : stub_(nullptr), srv_worker_(nullptr), 
    listener_ptr_(new lava_server::server_listener) {
    //lava env init
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("lava.server.thread", conf_buf, conf_len);
    std::string srv_thread_cnt(conf_buf);
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }
    int thread_cnt = 1;
    if (!srv_thread_cnt.empty())
        thread_cnt = std::stoi(srv_thread_cnt);
    assert(thread_cnt >= 1);
    init_environment(0, thread_cnt);
    log_info(lava_server_module, "internal lava server: worker-thread = %d", thread_cnt);

    //init thread exit status
    thread_exit_.store(false);
}

lava_server::~lava_server() {
    //release lava env
    clear_environment();
}

i_server_listener::auth_result lava_server::server_listener::on_authentication(__lv_in const char* username, 
    __lv_in const char* password) {
    auth_result result_flg = identity_ok;

    //read config and verify
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("lava.server.name", conf_buf, conf_len);
    if (0 != memcmp(conf_buf, username, conf_len)) {
        log_debug(lava_server_module, "lava server name not correct.[%s]", conf_buf);
        result_flg = username_error;
    }
    else {
        get_lava_config()->value("lava.server.pwd", conf_buf, conf_len);
        if (0 != memcmp(conf_buf, password, conf_len)) {
            log_debug(lava_server_module, "lava server password not correct.[%s]", conf_len);
            result_flg = password_error;
        }
        else
            log_debug(lava_server_module, "login success.");
    }

    //release buf
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }

    return result_flg;
}

void lava_server::server_listener::on_request(__lv_in i_message* msg) {
    if (nullptr == msg)
        return;
    if (req_msg_cnt_ > req_msg_limit_cnt_) {
        log_warn(lava_server_module, "lava message exceeds the maximum processing limit, \
            and has been discarded by server.");
        return;
    }
    msg->reference();
    get_lava_process()->put_lava_task(msg);
    ++req_msg_cnt_;
    //get_lava_server()->send_request(msg);
}

bool lava_server::start() {
    if (thread_exit_)
        return false;

    //server run
    if (nullptr == srv_worker_)
        //create worker thead and run
        srv_worker_ = std::make_shared<std::thread>(std::bind(&lava_server::run, this));

    return true;
}

bool lava_server::stop() {
    if (srv_worker_) {
        //send stop command and wait server worker stop
        thread_exit_.store(true);
        srv_worker_->join();
    }
    return true;
}

void lava_server::run() {
    if (nullptr == listener_ptr_.get()) {
        log_error(lava_server_module, "create lava server listener failed!");
        return;
    }
    i_auto_ptr<i_server_listener> i_listener_ptr = listener_ptr_.get();

    char *conf_buf = nullptr;
    int conf_len = 0;
    //get cluster flag
    get_lava_config()->value("lava.cluster.enable", conf_buf, conf_len);
    std::string cluster_flag(conf_buf);
    bool cluster_enable = false;
    if (!cluster_flag.empty()) {
        if (std::stoi(cluster_flag) > 0)
            cluster_enable = true;
    }

    //create lava server stub for message distribution
    get_lava_config()->value("lava.server.addr", conf_buf, conf_len);
    std::string server_ip(conf_buf);
    get_lava_config()->value("lava.server.port", conf_buf, conf_len);
    std::string server_port(conf_buf);
    get_lava_config()->value("lava.server.name", conf_buf, conf_len);
    std::string server_name(conf_buf);
    assert(!server_ip.empty());
    assert(!server_port.empty());
    assert(!server_name.empty());
    if (cluster_enable) {
        get_lava_config()->value("lava.cluster.addr", conf_buf, conf_len);
        std::string cluster_ip(conf_buf);
        get_lava_config()->value("lava.cluster.port", conf_buf, conf_len);
        std::string cluster_port(conf_buf);
        get_lava_config()->value("lava.cluster.type", conf_buf, conf_len);
        int cluster_type = std::stoi(conf_buf);
        assert(!cluster_ip.empty());
        assert(!cluster_port.empty());
        assert(cluster_type >= 0);
        stub_ = create_stub(server_name.c_str(), i_listener_ptr.get(), 
            server_ip.c_str(), server_port.c_str(), 
            (cluster_ip + ":" + cluster_port).c_str(), 
            server_name.c_str(), (lava::comm::lava_service_type)cluster_type);
        log_info(lava_server_module, "lava server run as cluster:[server_ip=%s, \
            server_port=%s, server_name = %s, cluster_ip = %s, cluster_port = %s, \
            cluster_type= %d].", server_ip.c_str(), server_port.c_str(), 
            server_name.c_str(), cluster_ip.c_str(), cluster_port.c_str(), cluster_type);
    }
    else {
        stub_ = create_stub(server_name.c_str(), i_listener_ptr.get(), 
            server_ip.c_str(), server_port.c_str());
        log_info(lava_server_module, "lava server run as singleton:[server_ip=%s, \
            server_port=%s, server_name=%s].", server_ip.c_str(), server_port.c_str(), 
            server_name.c_str());
    }

    //release config buf
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }

    //worker thread loop
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (thread_exit_)
            break;
    }
}

void lava_server::send_request(__lv_in i_message* msg) {
    if (nullptr == msg)
        return;
    msg->set_msg_type(MSG_TYPE_DATA_REPLY);
    const char* reply_to = msg->get_reply_to();
    msg->set_destination(reply_to, strlen(reply_to));
    stub_->reply_message(msg);
    msg->dereference();
    listener_ptr_->decrease_req_msg_cnt();
}

void lava_server::send_msg_publish(__lv_in i_message* msg, __lv_in const char *topic) {
    if ((nullptr == msg) || (nullptr == topic))
        return;
    stub_->publish_message(msg, topic);
}

}
}
