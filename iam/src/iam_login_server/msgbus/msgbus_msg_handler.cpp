#include "lava_utils_api.h"
#include "msgbus_server.h"
#include "msgbus_msg_handler.h"

extern iam::login_server::i_msgbus_server *get_msgbus_server();

typedef msgbus::VariantList sslist;
typedef msgbus::VariantMap ssmap;


bool msgbus_msg_handler::parse_verify_token_req(__lv_in const msgbus::Message &message, 
    __lv_out VerifyTokenReqPtr vfy_tkn_ptr) {
    if (nullptr == vfy_tkn_ptr)
        return false;

    //parse message header
    vfy_tkn_ptr->msg_header_ptr_ = msgbus_msg_handler::parse_header(message);
    if (!vfy_tkn_ptr->msg_header_ptr_.get()) {
        log_warn(msgbus_msg_handler_moduler, "token verify req message header parse failed!");
        return false;
    }

    /* parse message body */

    //parse req msg list
    msgbus::Message::Map reqMap;
    message.GetMap("xGtwTokenVerifyReq", reqMap);
    if (reqMap.empty()) {
        log_warn(msgbus_msg_handler_moduler, "token verify req message map parse failed!");
        return false;
    }

    msgbus::Message::List lstReq;
    reqMap.getList("List", lstReq);
    if (lstReq.empty()) {
        log_warn(msgbus_msg_handler_moduler, "token verify req message map list parse failed!");
        return false;
    }

    //parse req msg content map
    const ssmap* pMap = nullptr;
    msgbus::Message::List::const_iterator itLst = lstReq.begin();
    for (; itLst != lstReq.end(); itLst++) {
        if (!itLst->RefMap(pMap) || nullptr == pMap)
            continue;
        server_auth_req srv_auth_req;
        pMap->getString("userName", srv_auth_req.user_name_);
        pMap->getString("token", srv_auth_req.token_);
        vfy_tkn_ptr->vec_srv_auth_req_.push_back(srv_auth_req);
    }

    if (vfy_tkn_ptr->vec_srv_auth_req_.empty()) {
        log_warn(msgbus_msg_handler_moduler, "token verify req empty!");
        return false;
    }

    return true;
}

bool msgbus_msg_handler::parse_user_info_upd_fanout(__lv_in const msgbus::Message &message, 
    __lv_out FanoutUserInfoPtr fanout_user_info_ptr) {
    if (nullptr == fanout_user_info_ptr)
        return false;

    /* parse message body */

    //parse req msg list
    msgbus::Message::Map reqMap;
    message.GetMap("xUserInfoUpdFanout", reqMap);
    if (reqMap.empty()) {
        log_warn(msgbus_msg_handler_moduler, "user info upd fanout message map parse failed!");
        return false;
    }

    msgbus::Message::List lstReq;
    reqMap.getList("List", lstReq);
    if (lstReq.empty()) {
        log_warn(msgbus_msg_handler_moduler, "user info upd fanout message map list parse failed!");
        return false;
    }

    //parse req msg content map
    const ssmap* pMap = nullptr;
    msgbus::Message::List::const_iterator itLst = lstReq.begin();
    for (; itLst != lstReq.end(); itLst++) {
        if (!itLst->RefMap(pMap) || nullptr == pMap)
            continue;
        user_info u_info;
        pMap->getInt16("optFlg", u_info.opt_flag_);
        pMap->getString("userName", u_info.user_name_);
        pMap->getString("userPwd", u_info.user_pwd_);
        pMap->getString("userId", u_info.user_id_);
        pMap->getString("cliMacAddr", u_info.cli_mac_addr_);
        pMap->getString("cliVersion", u_info.cli_version_);
        fanout_user_info_ptr->push_back(u_info);
    }

    if (fanout_user_info_ptr->empty()) {
        log_warn(msgbus_msg_handler_moduler, "user info upd fanout message body empty!");
        return false;
    }

    return true;
}

bool msgbus_msg_handler::parse_token_info_sync_fanout(__lv_in const msgbus::Message &message, 
    __lv_out FanoutTkInfoPtr fanout_tkn_info_ptr) {
    if (nullptr == fanout_tkn_info_ptr)
        return false;

    /* parse message body */

    //parse req msg list
    msgbus::Message::Map reqMap;
    message.GetMap("xTokenInfoSyncFanout", reqMap);
    if (reqMap.empty()) {
        log_warn(msgbus_msg_handler_moduler, "token info sync fanout message map parse failed!");
        return false;
    }

    msgbus::Message::List lstReq;
    reqMap.getList("List", lstReq);
    if (lstReq.empty()) {
        log_warn(msgbus_msg_handler_moduler, "token info sync fanout message map list parse failed!");
        return false;
    }

    //parse req msg content map
    const ssmap* pMap = nullptr;
    msgbus::Message::List::const_iterator itLst = lstReq.begin();
    for (; itLst != lstReq.end(); itLst++) {
        if (!itLst->RefMap(pMap) || nullptr == pMap)
            continue;
        token_info tk_info;
        pMap->getString("token", tk_info.token_);
        pMap->getInt64("tokenTime", tk_info.token_time_);
        pMap->getString("userName", tk_info.user_name_);
        fanout_tkn_info_ptr->push_back(tk_info);
    }

    if (fanout_tkn_info_ptr->empty()) {
        log_warn(msgbus_msg_handler_moduler, "token info sync fanout message body empty!");
        return false;
    }

    return true;
}

std::shared_ptr<msgbus_msg_header> msgbus_msg_handler::parse_header(__lv_in const msgbus::Message &message) {
    std::shared_ptr<msgbus_msg_header> header(std::make_shared<msgbus_msg_header>());
    header->type_ = message.getType();
    header->messageid_ = message.getMessageID();
    header->subject_ = message.getSubject();
    header->reply_ = message.getReplyTo();
    return header;
}

bool msgbus_msg_handler::package_token_info_sync_fanout(__lv_in FanoutTkInfoPtr fanout_tkn_info_ptr, 
    __lv_out std::shared_ptr<msgbus::Message> msg) {
    //check input
    if ((nullptr == fanout_tkn_info_ptr) || fanout_tkn_info_ptr->empty() || (nullptr == msg))
        return false;

    //set header: type and login server id
    msg->setType(LOGIN_SRV_SYNC_TOKEN_FANOUT);
    msg->setSubject(get_msgbus_server()->get_server_id());

    //set body
    msgbus::VariantMap *bodyMap = msg->AddMap("xTokenInfoSyncFanout");
    if (bodyMap) {
        msgbus::VariantList *lst = bodyMap->AddList("List");
        if (lst) {
            for (auto &it : *fanout_tkn_info_ptr) {
                msgbus::VariantMap *map = lst->AddMap();
                if (map) {
                    map->setString("token", it.token_);
                    map->SetInt64("tokenTime", it.token_time_);
                    map->setString("userName", it.user_name_);
                }
            }
            return true;
        }
    }

    log_warn(msgbus_msg_handler_moduler, "[package_token_info_sync_fanout]:set fanout message body failed!");
    return false;
}
