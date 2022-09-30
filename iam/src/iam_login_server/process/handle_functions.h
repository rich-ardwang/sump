/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : handle_functions.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-16
*Desc        : define the business handling functions in this file, 
*              including lava, http, msgbus etc.
*History     : Add by Lei.Wang 2019-10-16
***************************************************************************************************/
#ifndef __HANDLE_FUNCTIONS_H__
#define __HANDLE_FUNCTIONS_H__
#include "lava_utils_api.h"
#include "lava_server.h"
#include "lava_process.h"
#include "msgbus_process.h"
#include "data_cache.h"

#define handle_functions_moduler        "iam.login_server.handle_functions"

using namespace lava::comm;


namespace iam {
namespace login_server {

//lava business functions
class lava_task_handle {
public:
    static void lava_msg_parse(__lv_in i_message* msg, __lv_in lava_process *process);
    static void user_login(__lv_in i_message* msg, __lv_in lava_process *process, 
        __lv_in ClientAuthReqPtr usr_ptr);
    static void server_verify(__lv_in i_message* msg, __lv_in lava_process *process, 
        __lv_in ServerAuthReqPtr srv_ptr);
    static void package_cli_ack_msg(__lv_in i_message* msg, __lv_in lava_process *process, 
        __lv_in ClientAuthAckPtr u_ack_ptr);
    static void package_srv_ack_msg(__lv_in i_message* msg, __lv_in lava_process *process, 
        __lv_in ServerAuthAckPtr srv_ack_ptr);
    static void lava_msg_reply(__lv_in i_message* msg);
    static void lava_msg_publish(__lv_in i_message* msg, __lv_in const char *topic);
};

//msgbus business functions
class msgbus_task_handle {
public:
    static void server_verify_token(__lv_in VerifyTokenReqPtr req_ptr, 
        __lv_in msgbus_process *process);
    static void userinfo_upd_fanout(__lv_in FanoutUserInfoPtr fout_ptr, 
        __lv_in msgbus_process *process);
    static void tkinfo_sync_fanout(__lv_in FanoutTkInfoPtr fout_ptr, 
        __lv_in msgbus_process *process);
    static void send_srv_vfy_token_ack(__lv_in VerifyTokenAckPtr ack_ptr);
};

}
}

#endif  /* __HANDLE_FUNCTIONS_H__ */
