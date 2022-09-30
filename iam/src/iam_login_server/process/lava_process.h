/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : lava_process.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-16
*Desc        : lava process take charge for handling message parsing, 
*              business processing, message sending and so on according 
*              lava protocol.
*History     : Add by Lei.Wang 2019-10-16
***************************************************************************************************/
#ifndef __LAVA_PROCESS_H__
#define __LAVA_PROCESS_H__
#include "lava_utils_api.h"
#include "lava_server.h"
#include "thread_pool.h"
#include "data_cache.h"
#include "server_msg_def.h"

#define lava_process_moduler        "iam.login_server.lava_process"
#define default_lava_pool_size      4
#define default_token_scan_time     60


namespace iam {
namespace login_server {

//interfaces of lava process
struct i_lava_process {
    virtual void put_lava_task(__lv_in i_message* msg) = 0;
};

//class lava_process definition
class lava_process : public lava::base::noncopyable, 
                     public i_lava_process {
public:
    lava_process();
    ~lava_process();

public:
    virtual void put_lava_task(__lv_in i_message* msg) override;

public:
    void commit_user_login(__lv_in i_message* msg, __lv_in ClientAuthReqPtr usr_ptr);
    void commit_server_verify(__lv_in i_message* msg, __lv_in ServerAuthReqPtr srv_ptr);
    void commit_pkg_cli_ack_msg(__lv_in i_message* msg, __lv_in ClientAuthAckPtr u_ack_ptr);
    void commit_pkg_srv_ack_msg(__lv_in i_message* msg, __lv_in ServerAuthAckPtr srv_ack_ptr);
    void commit_msg_reply(__lv_in i_message* msg);
    void commit_msg_publish(__lv_in i_message* msg, 
        __lv_in const char *topic = lava_kick_msg_topic);

private:
    bool start();
    bool stop();
    void run();

private:
    threadpool                      *task_pool_;
    std::atomic<bool>               token_timer_exit_;
    std::shared_ptr<std::thread>    token_timer_;
    int                             token_scan_time_;
};

}
}

#endif  /* __LAVA_PROCESS_H__ */
