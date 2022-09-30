/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_process.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-24
*Desc        : msgbus process take charge for handling business processing, 
*            : message packaging, sending and so on according msgbus protocol.
*History     : Add by Lei.Wang 2019-10-24
***************************************************************************************************/
#ifndef __MSGBUS_PROCESS_H__
#define __MSGBUS_PROCESS_H__
#include "lava_base.h"
#include "thread_pool.h"
#include "data_cache.h"
#include "msgbus_server.h"
#include "msgbus_type.h"
#include "server_msg_def.h"

#define msgbus_process_moduler        "iam.login_server.msgbus_process"
#define default_msgbus_pool_size      4


namespace iam {
namespace login_server {

//interfaces of msgbus process
struct i_msgbus_process {
    virtual void put_srv_verify_token(__lv_in VerifyTokenReqPtr req_ptr) = 0;
    virtual void put_user_info_upd_fanout(__lv_in FanoutUserInfoPtr u_info_upd_ptr) = 0;
    virtual void put_token_info_sync_fanout(__lv_in FanoutTkInfoPtr tk_info_sync_ptr) = 0;
    virtual void put_srv_vfy_token_ack(__lv_in VerifyTokenAckPtr ack_ptr) = 0;
};

//class msgbus_process definition
class msgbus_process : public lava::base::noncopyable, 
                       public i_msgbus_process {
public:
    msgbus_process();
    ~msgbus_process();

public:
    virtual void put_srv_verify_token(__lv_in VerifyTokenReqPtr req_ptr) override;
    virtual void put_user_info_upd_fanout(__lv_in FanoutUserInfoPtr u_info_upd_ptr) override;
    virtual void put_token_info_sync_fanout(__lv_in FanoutTkInfoPtr tk_info_sync_ptr) override;
    virtual void put_srv_vfy_token_ack(__lv_in VerifyTokenAckPtr ack_ptr) override;

private:
    threadpool                      *task_pool_;
};

}
}

#endif  /* __MSGBUS_PROCESS_H__ */
