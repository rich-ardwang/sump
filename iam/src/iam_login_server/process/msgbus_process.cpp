#include <string>
#include <assert.h>
#include "singleton.h"
#include "msgbus_process.h"
#include "handle_functions.h"

//need extern using out of this file
iam::login_server::i_msgbus_process *get_msgbus_process() {
    return singleton_lazy<iam::login_server::msgbus_process, 
        iam::login_server::i_msgbus_process>::instance();
}

void release_msgbus_process() {
    singleton_lazy<iam::login_server::msgbus_process, 
        iam::login_server::i_msgbus_process>::release();
}

extern iam::login_server::i_srv_cache *get_server_cache();


namespace iam {
namespace login_server {

msgbus_process::msgbus_process() {
    int pool_size = 0;
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("loginsrv.msgbus_task_pool_size", conf_buf, conf_len);
    std::string tmp(conf_buf);
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }
    if (!tmp.empty())
        pool_size = std::stoi(tmp);
    if (pool_size <= 0)
        pool_size = default_msgbus_pool_size;

    //create msgbus task thread pool
    task_pool_ = new threadpool(pool_size);
    assert(task_pool_);
}

msgbus_process::~msgbus_process() {
    //release task pools
    if (task_pool_) {
        delete task_pool_;
        task_pool_ = nullptr;
    }
}

void msgbus_process::put_srv_verify_token(__lv_in VerifyTokenReqPtr req_ptr) {
    task_pool_->commit(&msgbus_task_handle::server_verify_token, req_ptr, this);
}

void msgbus_process::put_user_info_upd_fanout(__lv_in FanoutUserInfoPtr u_info_upd_ptr) {
    task_pool_->commit(&msgbus_task_handle::userinfo_upd_fanout, u_info_upd_ptr, this);
}

void msgbus_process::put_token_info_sync_fanout(__lv_in FanoutTkInfoPtr tk_info_sync_ptr) {
    task_pool_->commit(&msgbus_task_handle::tkinfo_sync_fanout, tk_info_sync_ptr, this);
}

void msgbus_process::put_srv_vfy_token_ack(__lv_in VerifyTokenAckPtr ack_ptr) {
    task_pool_->commit(&msgbus_task_handle::send_srv_vfy_token_ack, ack_ptr);
}

}
}
