/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_msg_handler.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-23
*Desc        : handle msgbus messages, such as parse or package.
*History     : Add by Lei.Wang 2019-10-23
***************************************************************************************************/
#ifndef __MSGBUS_PARSER_H__
#define __MSGBUS_PARSER_H__
#include <string>
#include <vector>
#include <memory>
#include "lava_base.h"
#include "msgbus/message.h"
#include "common.h"
#include "msgbus_type.h"

#define msgbus_msg_handler_moduler       "iam.login_server.msgbus_msg_handler"


class msgbus_msg_handler {
public:
    static bool parse_verify_token_req(__lv_in const msgbus::Message &message, 
        __lv_out VerifyTokenReqPtr vfy_tkn_ptr);
    static bool parse_user_info_upd_fanout(__lv_in const msgbus::Message &message, 
        __lv_out FanoutUserInfoPtr fanout_user_info_ptr);
    static bool parse_token_info_sync_fanout(__lv_in const msgbus::Message &message, 
        __lv_out FanoutTkInfoPtr fanout_tkn_info_ptr);
    static bool package_token_info_sync_fanout(__lv_in FanoutTkInfoPtr fanout_tkn_info_ptr, 
        __lv_out std::shared_ptr<msgbus::Message> msg);

private:
    static std::shared_ptr<msgbus_msg_header> parse_header(__lv_in const msgbus::Message &message);
};

#endif /* __MSGBUS_PARSER_H__ */
