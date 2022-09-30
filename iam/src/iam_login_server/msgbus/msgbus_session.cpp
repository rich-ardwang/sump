#include "msgbus_session.h"
#include "msgbus_type.h"
#include "msgbus_helper.h"
#include "msgbus_msg_handler.h"
#include "msgbus_process.h"

extern lava_utils_api lava::utils::i_lava_config *get_lava_config();
extern iam::login_server::i_msgbus_process *get_msgbus_process();
extern iam::login_server::i_msgbus_server *get_msgbus_server();


void MsgbusSession::onMessage(const msgbus::Message &message) {
    if (!checkReplyQueue(message))
        return;

    int ret_code = kSuccess;
    std::string type = message.getType();
    if (type == GTW_SRV_VERIFY_TOKEN_REQ) {
        VerifyTokenReqPtr vfy_tkn_ptr(std::make_shared<verify_token_req>());
        bool ret = msgbus_msg_handler::parse_verify_token_req(message, vfy_tkn_ptr);
        if (ret)
            //commit task to msgbus process
            get_msgbus_process()->put_srv_verify_token(vfy_tkn_ptr);
        else {
            ret_code = kMsgParaseError;
            procError(message, ret_code);
        }
    }
    else
        log_warn(msgbus_session_moduler, "invalid type - %s!", type.c_str());
}

void MsgbusSession::onFanoutMessage(const msgbus::Message &message) {
    log_debug(msgbus_session_moduler, "[MsgbusSession::onFanoutMessage] - %s.", 
        MessageHelper::getMessageDescriptor(message).c_str());

    bool ret = false;
    std::string type = message.getType();
    if (DB_SRV_USER_INFO_UPD_FANOUT == type) {
        FanoutUserInfoPtr u_info_ptr(std::make_shared<std::vector<user_info> >());
        ret = msgbus_msg_handler::parse_user_info_upd_fanout(message, u_info_ptr);
        if (ret)
            //commit task to msgbus process
            get_msgbus_process()->put_user_info_upd_fanout(u_info_ptr);
    }
    else if (LOGIN_SRV_SYNC_TOKEN_FANOUT == type) {
        std::string srvid = message.getSubject();
        if (srvid != get_msgbus_server()->get_server_id()) {
            FanoutTkInfoPtr tk_info_ptr(std::make_shared<std::vector<token_info> >());
            ret = msgbus_msg_handler::parse_token_info_sync_fanout(message, tk_info_ptr);
            if (ret)
                //commit task to msgbus process
                get_msgbus_process()->put_token_info_sync_fanout(tk_info_ptr);
        }
    }
    else
        log_warn(msgbus_session_moduler, "[MsgbusSession::onFanoutMessage] - invalid type - %s!", type.c_str());
}

bool MsgbusSession::checkReplyQueue(const msgbus::Message &message) {
    const std::string &reply = message.getReplyTo();
    if (reply.empty()) {
        log_warn(msgbus_session_moduler, "[MsgbusSession::checkReplyQueue] - reply empty!");
        return false;
    }
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("msgbus.direct.listen_queue", conf_buf, conf_len);
    std::string listenQueue(conf_buf);
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }
    if (reply == listenQueue) {
        log_warn(msgbus_session_moduler, "[MsgbusSession::checkReplyQueue] - wrong reply queue!");
        return false;
    }
    return true;
}

void MsgbusSession::procError(const msgbus::Message &message, int errCode) {
    switch(errCode) {
    case kMsgParaseError:
        procMsgParaseErr(message, true);
        break;
    case kServerStatusError:
        procServerStatusErr(message, true);
        break;
    default:
        log_warn(msgbus_session_moduler, "invalid errorCode.%d", errCode);
        break;
    }
}

void MsgbusSession::procMsgParaseErr(const msgbus::Message &msg, bool isReply) {
    msgbus::Message response;
    msgbus::Message::Map *head = response.AddMap("AckMsgHead");
    head->SetInt16("retcode", kMsgParaseError);
    head->setString("desc", "request message parse failed!");

    response.SetType(msg.GetType());
    response.SetMessageID(msg.GetMessageID());
    response.SetReplyTo(msg.GetReplyTo());

    log_warn(msgbus_session_moduler, "request message body parse fail, message content:");
    log_warn(msgbus_session_moduler, "%s", MessageHelper::getMessageDescriptor(msg).c_str());

    if (isReply)
        con_->send(msg.GetReplyTo(), response);
}

void MsgbusSession::procServerStatusErr(const msgbus::Message &msg, bool isReply) {
    msgbus::Message response;
    msgbus::Message::Map *head = response.AddMap("AckMsgHead");
    head->SetInt16("retcode", kServerStatusError);
    head->setString("desc", "server status error!");

    response.SetType(msg.GetType());
    response.SetMessageID(msg.GetMessageID());
    response.SetReplyTo(msg.GetReplyTo());

    log_warn(msgbus_session_moduler, "server status error, message content:");
    log_warn(msgbus_session_moduler, "%s", MessageHelper::getMessageDescriptor(msg).c_str());

    if (isReply)
        con_->send(msg.GetReplyTo(), response);
}
