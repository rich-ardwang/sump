/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : lava_server.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-25
*Desc        : lava server implementation.
*History     : Add by Lei.Wang 2019-09-25
***************************************************************************************************/
#ifndef __LAVA_SERVER_H__
#define __LAVA_SERVER_H__
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include "lava_utils_api.h"
#include "lava_box_api.h"

#define lava_server_module          "iam.login_server.lava_server"
#define default_limit_msg_cnt       1500

extern lava_utils_api lava::utils::i_lava_config *get_lava_config();

using namespace lava::base;
using namespace lava::comm;
using namespace lava::box;
using namespace lava::proto;


namespace iam {
namespace login_server {

//interfaces of lava server
struct i_lava_server {
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual void send_request(__lv_in i_message* msg) = 0;
    virtual void send_msg_publish(__lv_in i_message* msg, __lv_in const char *topic) = 0;
};

//class lava_server definition
class lava_server : public i_lava_server {
private:
    class server_listener : public i_server_listener {
    public:
        inline server_listener() {
            //get lava msg limit count
            char *conf_buf = nullptr;
            int conf_len = 0;
            get_lava_config()->value("loginsrv.lava_msg_handle_limit", conf_buf, conf_len);
            std::string s_limit_cnt(conf_buf);
            if (conf_buf) {
                delete[] conf_buf;
                conf_buf = nullptr;
            }
            if (s_limit_cnt.empty())
                req_msg_limit_cnt_ = default_limit_msg_cnt;
            else
                req_msg_limit_cnt_ = std::stoi(s_limit_cnt);

            //init request message count
            req_msg_cnt_.store(0);
        }

    public:
        inline void decrease_req_msg_cnt() {
            --req_msg_cnt_;
        }

    private:
        virtual auth_result on_authentication(__lv_in const char* username, 
            __lv_in const char* password) override;
        virtual void on_request(__lv_in i_message* msg) override;
        virtual void on_subscribe(__lv_in const char* topic) override {}
        virtual void on_unsubscribe(__lv_in const char* topic) override {}

    private:
        int32_t                   req_msg_limit_cnt_;
        std::atomic<int32_t>      req_msg_cnt_;
        i_object_impl(std::atomic<int32_t>)
    };

public:
    lava_server();
    ~lava_server();

public:
    virtual bool start() override;
    virtual bool stop() override;
    virtual void send_request(__lv_in i_message* msg) override;
    virtual void send_msg_publish(__lv_in i_message* msg, __lv_in const char *topic) override;

private:
    void run();

private:
    i_auto_ptr<lava::box::i_stub>               stub_;
    std::atomic<bool>                           thread_exit_;
    std::shared_ptr<std::thread>                srv_worker_;
    i_auto_ptr<server_listener>                 listener_ptr_;
};

}
}

#endif  /* __LAVA_SERVER_H__ */
