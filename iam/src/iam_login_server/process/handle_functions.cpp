#include "handle_functions.h"
#include "msgbus_msg_handler.h"
#include "common.h"

extern iam::login_server::i_lava_server *get_lava_server();
extern iam::login_server::i_srv_cache *get_server_cache();
extern iam::login_server::i_msgbus_server *get_msgbus_server();


namespace iam {
namespace login_server {

void lava_task_handle::lava_msg_parse(__lv_in i_message* msg, 
        __lv_in lava_process *process) {
    //check input
    if (nullptr == msg)
        return;
    if (nullptr == process) {
        msg->dereference();
        return;
    }

    //get and check message body
    i_message_body *body = msg->get_message_body();
    if (nullptr == body) {
        log_warn(handle_functions_moduler, "[lava_msg_parse]:get lava message body pointer failed!");
        msg->dereference();
        return;
    }
    if ((body->has_field(lava_msg_body_identity_flg) != i_message_body::INT) || 
        (body->has_field(lava_msg_body_function_id) != i_message_body::INT) || 
        (body->has_field(lava_msg_body_query_data) != i_message_body::STRING)) {
        log_warn(handle_functions_moduler, "parse lava message body failed!");
        msg->dereference();
        return;
    }

    //parse identity flag and from now on this param is not used
    int ident_flg = body->get_int(lava_msg_body_identity_flg);

    //parse function id
    int func_id = body->get_int(lava_msg_body_function_id);

    //parse the query data from lava message body
    std::string qry_data(body->get_string(lava_msg_body_query_data));
    if (qry_data.empty()) {
        log_warn(handle_functions_moduler, "parse client query data failed!");
        msg->dereference();
        return;
    }

/*
    //output query info
    log_debug(handle_functions_moduler, "user login query info:{ identity_flg:%d, function_id:%d, query_data:[%s]. }", 
        ident_flg, func_id, qry_data.c_str());
*/
    //parse query params and do the business
    if (1 == func_id) {
        //create client auth req object
        ClientAuthReqPtr u_ptr(std::make_shared<client_auth_req>());

        std::vector<std::string> vec_data;
        server_utils::split(qry_data, parse_message_separator, vec_data);
        int idx = 0;
        for (auto &it : vec_data) {
            switch (idx) {
            case 0:
                u_ptr->user_name_ = it;
                break;
            case 1:
                u_ptr->user_pwd_ = it;
                break;
            case 2:
                u_ptr->cli_mac_addr_ = it;
                break;
            case 3:
                u_ptr->cli_version_ = it;
                break;
            default:
                break;
            }
            idx++;
        }

        //do user login
        process->commit_user_login(msg, u_ptr);
    }
    else if (2 == func_id) {
        //create server auth req object
        ServerAuthReqPtr srv_ptr(std::make_shared<server_auth_req>());

        std::vector<std::string> vec_data;
        server_utils::split(qry_data, parse_message_separator, vec_data);
        int idx = 0;
        for (auto &it : vec_data) {
            switch (idx) {
            case 0:
                srv_ptr->user_name_ = it;
                break;
            case 1:
                srv_ptr->token_ = it;
                break;
            default:
                break;
            }
            idx++;
        }

        //do server verify
        process->commit_server_verify(msg, srv_ptr);
    }
}

void lava_task_handle::user_login(__lv_in i_message* msg, __lv_in lava_process *process, 
    __lv_in ClientAuthReqPtr usr_ptr) {
    //check input
    if (nullptr == msg)
        return;

    if (nullptr == process) {
        msg->dereference();
        return;
    }

    if (nullptr == usr_ptr) {
        log_warn(handle_functions_moduler, "[user_login]:usr_ptr is nullptr!");
        msg->dereference();
        return;
    }

    if (usr_ptr->user_name_.empty() || usr_ptr->user_pwd_.empty()) {
        log_warn(handle_functions_moduler, "[user_login]:client login req data empty!");
        msg->dereference();
        return;
    }

    //create client auth ack object
    ClientAuthAckPtr cli_ack_ptr(std::make_shared<client_auth_ack>());
    cli_ack_ptr->user_name_ = usr_ptr->user_name_;

    //get user info and check
    user_info uinfo;
    bool ret = get_server_cache()->get_user_info(usr_ptr->user_name_, uinfo);
    if (!ret) {
        cli_ack_ptr->result_code_ = lava_user_login_name_fail_code;
        cli_ack_ptr->result_msg_ = lava_user_login_name_fail;
    }
    else {
        //verify user password
        if (uinfo.user_pwd_ != usr_ptr->user_pwd_) {
            cli_ack_ptr->result_code_ = lava_user_login_pwd_fail_code;
            cli_ack_ptr->result_msg_ = lava_user_login_pwd_fail;
        }
        else {
            //set user login ack data
            char *conf_buf = nullptr;
            int conf_len = 0;
            get_lava_config()->value("dds.server.addr", conf_buf, conf_len);
            cli_ack_ptr->dds_srv_addr_ = conf_buf;
            get_lava_config()->value("dds.server.port", conf_buf, conf_len);
            cli_ack_ptr->dds_srv_port_ = conf_buf;
            get_lava_config()->value("gateway.server.addr", conf_buf, conf_len);
            cli_ack_ptr->gtw_srv_addr_ = conf_buf;
            get_lava_config()->value("gateway.server.port", conf_buf, conf_len);
            cli_ack_ptr->gtw_srv_port_ = conf_buf;
            get_lava_config()->value("msgbus.fanout.listen_queue", conf_buf, conf_len);
            std::string fout_qname = conf_buf;
            if (conf_buf) {
                delete[] conf_buf;
                conf_buf = nullptr;
            }
            cli_ack_ptr->user_id_ = uinfo.user_id_;
            cli_ack_ptr->result_code_ = lava_user_login_success_code;
            cli_ack_ptr->result_msg_ = lava_user_login_success;
            cli_ack_ptr->server_sys_time_ = server_utils::get_current_system_time();

            //query token by user name, if the token is still valid, 
            //no need to generate
            std::string token;
            ret = get_server_cache()->get_token_by_uname(cli_ack_ptr->user_name_, token);
            if (ret) {
                cli_ack_ptr->token_ = token;
            }
            else {
                //generate token using user info
                token_info tkn_info;
                ret = get_server_cache()->generate_token(uinfo, tkn_info);
                if (!ret) {
                    cli_ack_ptr->dds_srv_addr_ = "";
                    cli_ack_ptr->dds_srv_port_ = "";
                    cli_ack_ptr->gtw_srv_addr_ = "";
                    cli_ack_ptr->gtw_srv_port_ = "";
                    cli_ack_ptr->user_id_ = "";
                    cli_ack_ptr->server_sys_time_ = "";
                    cli_ack_ptr->result_code_ = login_server_handle_fail_code;
                    cli_ack_ptr->result_msg_ = login_server_handle_fail;
                }
                else {
                    cli_ack_ptr->token_ = tkn_info.token_;

                    //insert token info locally
                    get_server_cache()->add_token_info(tkn_info);

                    //fanout user token sync message via msgbus
                    FanoutTkInfoPtr fout_ptr(std::make_shared<std::vector<token_info> >());
                    fout_ptr->push_back(tkn_info);
                    std::shared_ptr<msgbus::Message> msgbus_msg_ptr(std::make_shared<msgbus::Message>());
                    bool ret = msgbus_msg_handler::package_token_info_sync_fanout(fout_ptr, msgbus_msg_ptr);
                    if ((ret) && (nullptr != msgbus_msg_ptr))
                        get_msgbus_server()->fanout(fout_qname, msgbus_msg_ptr);
                    else
                        log_warn(handle_functions_moduler, "sync token info failed!");
                }
            }
        }
    }

    //send reply message to user client
    process->commit_pkg_cli_ack_msg(msg, cli_ack_ptr);
}

void lava_task_handle::server_verify(__lv_in i_message* msg, __lv_in lava_process *process, 
    __lv_in ServerAuthReqPtr srv_ptr) {
    //check input
    if (nullptr == msg)
        return;

    if (nullptr == process) {
        msg->dereference();
        return;
    }

    if (nullptr == srv_ptr) {
        log_warn(handle_functions_moduler, "[server_verify]:srv_ptr is nullptr!");
        msg->dereference();
        return;
    }

    if (srv_ptr->user_name_.empty() || srv_ptr->token_.empty()) {
        log_warn(handle_functions_moduler, "[server_verify]:server verify req data empty!");
        msg->dereference();
        return;
    }

    //create server verify ack object and set part of return values
    ServerAuthAckPtr srv_ack_ptr(std::make_shared<server_auth_ack>());
    srv_ack_ptr->user_name_ = srv_ptr->user_name_;
    srv_ack_ptr->token_ = srv_ptr->token_;

    //verify user name
    user_info u_info;
    if (get_server_cache()->get_user_info(srv_ptr->user_name_, u_info)) {
        //verify user token
        token_info tkn_info;
        bool ret = get_server_cache()->get_token_info(srv_ptr->token_, tkn_info);
        if (ret) {
            srv_ack_ptr->result_code_ = lava_server_verify_sucess_code;
            srv_ack_ptr->result_msg_ = lava_server_verify_success;
            srv_ack_ptr->server_sys_time_ = server_utils::get_current_system_time();
            srv_ack_ptr->user_id_ = u_info.user_id_;
        }
        else {
            srv_ack_ptr->result_code_ = lava_server_verify_fail_code;
            srv_ack_ptr->result_msg_ = lava_server_verify_fail;
        }
    }
    else {
        srv_ack_ptr->result_code_ = lava_server_verify_invalid_user_code;
        srv_ack_ptr->result_msg_ = lava_server_verify_invalid_user;
    }

    //send reply message to dds server
    process->commit_pkg_srv_ack_msg(msg, srv_ack_ptr);
}

void lava_task_handle::package_cli_ack_msg(__lv_in i_message* msg, 
    __lv_in lava_process *process, __lv_in ClientAuthAckPtr u_ack_ptr) {
    //check input
    if (nullptr == msg)
        return;

    if (nullptr == process) {
        msg->dereference();
        return;
    }

    if (nullptr == u_ack_ptr) {
        log_warn(handle_functions_moduler, "[package_cli_ack_msg]:u_ack_ptr is nullptr!");
        msg->dereference();
        return;
    }

    if (u_ack_ptr->user_name_.empty()) {
        log_warn(handle_functions_moduler, "[package_cli_ack_msg]:client login ack data empty!");
        msg->dereference();
        return;
    }

    //set ack msg
    i_message_body *body = msg->get_message_body();
    if (nullptr == body) {
        log_warn(handle_functions_moduler, "[package_cli_ack_msg]:get lava message body pointer failed!");
        msg->dereference();
        return;
    }
    body->add_int(lava_msg_body_handle_ret_code, u_ack_ptr->result_code_);
    body->add_string(lava_msg_body_handle_ret_msg, u_ack_ptr->result_msg_.c_str());
    std::string ret_body;
    ret_body += u_ack_ptr->user_name_;
    ret_body += parse_message_separator;
    ret_body += u_ack_ptr->user_id_;
    ret_body += parse_message_separator;
    ret_body += u_ack_ptr->token_;
    ret_body += parse_message_separator;
    ret_body += u_ack_ptr->server_sys_time_;
    ret_body += parse_message_separator;
    ret_body += u_ack_ptr->dds_srv_addr_;
    ret_body += parse_message_separator;
    ret_body += u_ack_ptr->dds_srv_port_;
    ret_body += parse_message_separator;
    ret_body += u_ack_ptr->gtw_srv_addr_;
    ret_body += parse_message_separator;
    ret_body += u_ack_ptr->gtw_srv_port_;
    body->add_string(lava_msg_body_ack_data, ret_body.c_str());

/*
    //output ack info
    log_debug(handle_functions_moduler, "user login ack info:{ ret_code:%d, ret_msg:%s, ret_body:[%s]. }", 
        u_ack_ptr->result_code_, u_ack_ptr->result_msg_.c_str(), ret_body.c_str());
*/

    //reply to client
    process->commit_msg_reply(msg);
}

void lava_task_handle::package_srv_ack_msg(__lv_in i_message* msg, 
    __lv_in lava_process *process, __lv_in ServerAuthAckPtr srv_ack_ptr) {
    //check input
    if (nullptr == msg)
        return;

    if (nullptr == process) {
        msg->dereference();
        return;
    }

    if (nullptr == srv_ack_ptr) {
        log_warn(handle_functions_moduler, "[package_srv_ack_msg]:srv_ack_ptr is nullptr!");
        msg->dereference();
        return;
    }

    if (srv_ack_ptr->user_name_.empty()) {
        log_warn(handle_functions_moduler, "[package_srv_ack_msg]:server verify ack data empty!");
        msg->dereference();
        return;
    }

    //set ack msg
    i_message_body *body = msg->get_message_body();
    if (nullptr == body) {
        log_warn(handle_functions_moduler, "[package_srv_ack_msg]:get lava message body pointer failed!");
        msg->dereference();
        return;
    }
    body->add_int(lava_msg_body_handle_ret_code, srv_ack_ptr->result_code_);
    body->add_string(lava_msg_body_handle_ret_msg, srv_ack_ptr->result_msg_.c_str());
    std::string ret_body;
    ret_body += srv_ack_ptr->user_name_;
    ret_body += parse_message_separator;
    ret_body += srv_ack_ptr->user_id_;
    ret_body += parse_message_separator;
    ret_body += srv_ack_ptr->token_;
    ret_body += parse_message_separator;
    ret_body += srv_ack_ptr->server_sys_time_;
    body->add_string(lava_msg_body_ack_data, ret_body.c_str());

/*
    //output ack info
    log_debug(handle_functions_moduler, "server verify ack info:{ ret_code:%d, ret_msg:%s, ret_body:[%s]. }", 
        srv_ack_ptr->result_code_, srv_ack_ptr->result_msg_.c_str(), ret_body.c_str());
*/

    //reply to other server
    process->commit_msg_reply(msg);

}

void lava_task_handle::lava_msg_reply(__lv_in i_message* msg) {
    get_lava_server()->send_request(msg);
}

void lava_task_handle::lava_msg_publish(__lv_in i_message* msg, __lv_in const char *topic) {
    get_lava_server()->send_msg_publish(msg, topic);
}

void msgbus_task_handle::server_verify_token(__lv_in VerifyTokenReqPtr req_ptr, 
    __lv_in msgbus_process *process) {
    //check input
    if ((nullptr == req_ptr) || (nullptr == process))
        return;

    if (req_ptr->vec_srv_auth_req_.empty())
        return;

    //create verify token ack ptr and get the message header
    VerifyTokenAckPtr ack_ptr(std::make_shared<verify_token_ack>());
    ack_ptr->msg_header_ptr_ = req_ptr->msg_header_ptr_;

    for (auto &it : req_ptr->vec_srv_auth_req_) {
        //create tmp server auth ack and set part of return values
        server_auth_ack srv_auth_ack;
        srv_auth_ack.token_ = it.token_;
        srv_auth_ack.user_name_ = it.user_name_;

        //verify every user name
        user_info u_info;
        if (get_server_cache()->get_user_info(it.user_name_, u_info)) {
            //verify every user token
            token_info tkn_info;
            bool ret = get_server_cache()->get_token_info(it.token_, tkn_info);
            if (ret) {
                srv_auth_ack.result_code_ = msgbus_server_verify_sucess_code;
                srv_auth_ack.result_msg_ = msgbus_server_verify_success;
                srv_auth_ack.server_sys_time_ = server_utils::get_current_system_time();
                srv_auth_ack.user_id_ = u_info.user_id_;
            }
            else {
                srv_auth_ack.result_code_ = msgbus_server_verify_fail_code;
                srv_auth_ack.result_msg_ = msgbus_server_verify_fail;
            }
        }
        else {
            srv_auth_ack.result_code_ = msgbus_server_verify_invalid_user_code;
            srv_auth_ack.result_msg_ = msgbus_server_verify_invalid_user;
        }

        ack_ptr->vec_srv_auth_ack_.push_back(srv_auth_ack);
    }

    //send reply to gateway server
    if (!ack_ptr->vec_srv_auth_ack_.empty())
        process->put_srv_vfy_token_ack(ack_ptr);
}

void msgbus_task_handle::userinfo_upd_fanout(__lv_in FanoutUserInfoPtr fout_ptr, 
    __lv_in msgbus_process *process) {
    //check input
    if ((nullptr == fout_ptr) || (nullptr == process))
        return;

    //update or delete user info according to opt_flag
    for (auto &it : *fout_ptr)
        get_server_cache()->update_user_info(it, it.opt_flag_);
}

void msgbus_task_handle::tkinfo_sync_fanout(__lv_in FanoutTkInfoPtr fout_ptr, 
    __lv_in msgbus_process *process) {
    //check input
    if ((nullptr == fout_ptr) || (nullptr == process))
        return;

    //add token info
    for (auto &it : *fout_ptr)
        get_server_cache()->add_token_info(it);
}

void msgbus_task_handle::send_srv_vfy_token_ack(__lv_in VerifyTokenAckPtr ack_ptr) {
    if (nullptr == ack_ptr)
        return;

    //create message obj and set values
    std::shared_ptr<msgbus::Message> msg(std::make_shared<msgbus::Message>());
    msg->setType(ack_ptr->msg_header_ptr_->type_);
    msg->setMessageID(ack_ptr->msg_header_ptr_->messageid_);

    //set ack header
    msgbus::VariantMap *header = msg->AddMap("AckMsgHead");
    if (!header) {
        log_warn(handle_functions_moduler, "set ack header failed!");
        return;
    }
    header->setInt16("retcode", kSuccess);
    header->setString("desc", "verify token handle success.");

    //set ack body
    msgbus::VariantMap *ackBody = msg->AddMap("AckMsgBody");
    if (!ackBody) {
        log_warn(handle_functions_moduler, "set ack body failed!");
        return;
    }

    msgbus::VariantList *lst = ackBody->AddList("List");
    if (!lst) {
        log_warn(handle_functions_moduler, "set ack body failed!");
        return;
    }

    for (auto &it : ack_ptr->vec_srv_auth_ack_) {
        msgbus::VariantMap *map = lst->AddMap();
        if (map) {
            map->SetInt16("verifyRet", it.result_code_);
            map->setString("verifyMsg", it.result_msg_);
            map->setString("userName", it.user_name_);
            map->setString("userId", it.user_id_);
            map->setString("token", it.token_);
            map->setString("serverTime", it.server_sys_time_);
        }
    }

    //send reply to gateway server
    get_msgbus_server()->send(ack_ptr->msg_header_ptr_->reply_, msg);
}

}
}
