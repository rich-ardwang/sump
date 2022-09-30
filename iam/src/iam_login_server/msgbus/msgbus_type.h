/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_type.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-17
*Desc        : define the message type of msgbus.
*History     : Add by Lei.Wang 2019-09-17
*            : Mod by Lei.Wang 2019-10-23
***************************************************************************************************/
#ifndef __MSGBUS_TYPE_H__
#define __MSGBUS_TYPE_H__
#include <string>
#include <vector>
#include "data_cache.h"

using namespace iam::login_server;


/*---------------------------msgbus message def--------------------------------*/
enum emMsgbusType {
    kDirectMsgbus = 1,
    kFanoutMsgbus = 2
};

struct msgbus_msg_header {
    std::string     type_;
    std::string     messageid_;
    std::string     subject_;
    std::string     reply_;
};

//token verify request of gateway server
struct verify_token_req {
    inline verify_token_req() : msg_header_ptr_(std::make_shared<msgbus_msg_header>()) {}

    std::vector<server_auth_req>            vec_srv_auth_req_;
    std::shared_ptr<msgbus_msg_header>      msg_header_ptr_;
};

//token verify response to gateway server
struct verify_token_ack {
    inline verify_token_ack() : msg_header_ptr_(std::make_shared<msgbus_msg_header>()) {}

    std::vector<server_auth_ack>            vec_srv_auth_ack_;
    std::shared_ptr<msgbus_msg_header>      msg_header_ptr_;
};

//handling result message code
enum emMsgCode {
    kSuccess = 0,
    kMsgParaseError,
    kServerStatusError,
};

//verify_token_req pointer
typedef std::shared_ptr<verify_token_req>           VerifyTokenReqPtr;

//verify_token_ack pointer
typedef std::shared_ptr<verify_token_ack>           VerifyTokenAckPtr;

//fanout message notify to update user info
typedef std::shared_ptr<std::vector<user_info> >    FanoutUserInfoPtr;

//fanout message notify to sync token info
typedef std::shared_ptr<std::vector<token_info> >   FanoutTkInfoPtr;

//token verify request coming from gateway server
#define GTW_SRV_VERIFY_TOKEN_REQ        "80100"

//token info sync fanout coming from login server 
//itself and other instances
#define LOGIN_SRV_SYNC_TOKEN_FANOUT     "80101"

//user info update fanout coming from db server
#define DB_SRV_USER_INFO_UPD_FANOUT     "80102"

#endif /* __MSGBUS_TYPE_H__ */
