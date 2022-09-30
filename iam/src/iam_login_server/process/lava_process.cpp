#include <string>
#include <assert.h>
#include "singleton.h"
#include "lava_process.h"
#include "handle_functions.h"

//need extern using out of this file
iam::login_server::i_lava_process *get_lava_process() {
    return singleton_lazy<iam::login_server::lava_process, 
        iam::login_server::i_lava_process>::instance();
}

void release_lava_process() {
    singleton_lazy<iam::login_server::lava_process, 
        iam::login_server::i_lava_process>::release();
}

extern iam::login_server::i_srv_cache *get_server_cache();

namespace iam {
namespace login_server {

lava_process::lava_process() : task_pool_(nullptr), token_scan_time_(0) {
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("loginsrv.lava_task_pool_size", conf_buf, conf_len);
    std::string pl_sz(conf_buf);
    int pool_size = 0;
    if (!pl_sz.empty())
        pool_size = std::stoi(pl_sz);
    if (pool_size <= 0)
        pool_size = default_lava_pool_size;
    get_lava_config()->value("loginsrv.token.scan_time", conf_buf, conf_len);
    std::string sc_tm(conf_buf);
    if (!sc_tm.empty())
        token_scan_time_ = std::stoi(sc_tm);
    if (token_scan_time_ <= 0)
        token_scan_time_ = default_token_scan_time;
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }

    //create lava task thread pool
    task_pool_ = new threadpool(pool_size);
    assert(task_pool_);

    //init timer exit flag
    token_timer_exit_.store(false);

    //start worker thread to handle token update
    start();
}

lava_process::~lava_process() {
    //release task pools
    if (task_pool_) {
        delete task_pool_;
        task_pool_ = nullptr;
    }

    //stop worker thread
    stop();
}

void lava_process::put_lava_task(__lv_in i_message* msg) {
    task_pool_->commit(&lava_task_handle::lava_msg_parse, msg, this);
}

void lava_process::commit_user_login(__lv_in i_message* msg, 
    __lv_in ClientAuthReqPtr usr_ptr) {
    task_pool_->commit(&lava_task_handle::user_login, msg, this, usr_ptr);
}

void lava_process::commit_server_verify(__lv_in i_message* msg, 
    __lv_in ServerAuthReqPtr srv_ptr) {
    task_pool_->commit(&lava_task_handle::server_verify, msg, this, srv_ptr);
}

void lava_process::commit_pkg_cli_ack_msg(__lv_in i_message* msg, 
    __lv_in ClientAuthAckPtr u_ack_ptr) {
    task_pool_->commit(&lava_task_handle::package_cli_ack_msg, msg, this, u_ack_ptr);
}

void lava_process::commit_pkg_srv_ack_msg(__lv_in i_message* msg, 
    __lv_in ServerAuthAckPtr srv_ack_ptr) {
    task_pool_->commit(&lava_task_handle::package_srv_ack_msg, msg, this, srv_ack_ptr);
}

void lava_process::commit_msg_reply(__lv_in i_message* msg) {
    task_pool_->commit(&lava_task_handle::lava_msg_reply, msg);
}

void lava_process::commit_msg_publish(__lv_in i_message* msg, 
    __lv_in const char *topic/* = lava_kick_msg_topic */) {
    task_pool_->commit(&lava_task_handle::lava_msg_publish, msg, topic);
}

bool lava_process::start() {
    if (token_timer_exit_)
        return false;

    //server run
    if (nullptr == token_timer_)
        //create token timer thead and run
        token_timer_ = std::make_shared<std::thread>(std::bind(&lava_process::run, this));

    return true;
}

bool lava_process::stop() {
    if (token_timer_) {
        //send stop command and wait timer thread stop
        token_timer_exit_.store(true);
        token_timer_->join();
    }
    return true;
}

void lava_process::run() {
    //token timer thread loop
    while (true) {
        if (token_timer_exit_)
            break;

        //clean invalid tokens
        get_server_cache()->remove_all_invalid_token_info();

        std::this_thread::sleep_for(std::chrono::seconds(token_scan_time_));
    }
}

}
}
