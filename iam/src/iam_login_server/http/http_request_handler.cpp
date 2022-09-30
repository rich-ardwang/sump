#include <fstream>
#include <sstream>
#include <string>
#include <cJSON.h>
#include "http_reply.h"
#include "http_request.h"
#include "http_request_handler.h"
#include "lava_utils_api.h"
#include "server_msg_def.h"

extern iam::login_server::i_srv_cache *get_server_cache();


namespace http {
namespace server {

request_handler::request_handler(const std::string &root_path, const std::string &srv_name, 
    const std::string &auth_pwd) : root_path_(root_path), 
                                   http_srv_name_(srv_name), 
                                   http_auth_pwd_(auth_pwd) {}

void request_handler::handle_request(const request& req, reply& rep) {
    //create http auth ack object and set default values
    HttpAuthAckPtr ack_ptr(std::make_shared<iam::login_server::server_auth_ack>());
    ack_ptr->result_code_ = http_server_verify_parse_url_fail_code;
    ack_ptr->result_msg_ = http_server_verify_parse_url_fail;

    //decode url to path.
    std::string request_path;
    if (!url_decode(req.uri, request_path)) {
        rep = reply::stock_reply(reply::bad_request, ack_ptr);
        return;
    }

    //request path must be absolute and not contain "..".
    if (request_path.empty() || request_path[0] != '/' 
        || request_path.find("..") != std::string::npos) {
        rep = reply::stock_reply(reply::bad_request, ack_ptr);
        return;
    }

    //if path ends in slash and then remove it.
    size_t rq_size = request_path.size();
    if (request_path[rq_size - 1] == '/')
        request_path = request_path.substr(0, rq_size - 1);

    //get the root_name and request_type
    std::size_t last_slash_pos = request_path.find_last_of("/");
    std::size_t first_slash_pos = request_path.find_first_of("/");
    if (last_slash_pos == first_slash_pos) {
        rep = reply::stock_reply(reply::bad_request, ack_ptr);
        return;
    }
    std::string root_path = request_path.substr(1, last_slash_pos - 1);
    std::string req_type = request_path.substr(last_slash_pos + 1);;

    //check root path
    if (root_path_ != root_path) {
        ack_ptr->result_code_ = http_server_verify_req_type_fail_code;
        ack_ptr->result_msg_ = http_server_verify_req_type_fail;
        rep = reply::stock_reply(reply::bad_request, ack_ptr);
        return;
    }

    //handle business
    if (http_request_type == req_type) {
        //parse request parameters
        HttpAuthReqPtr auth_req_ptr(std::make_shared<token_auth_req>());
        bool ret = parse_json_tk_vfy_req(req.content, auth_req_ptr);
        if (!ret) {
            ack_ptr->result_code_ = http_server_verify_req_param_fail_code;
            ack_ptr->result_msg_ = http_server_verify_req_param_fail;
            rep = reply::stock_reply(reply::bad_request, ack_ptr);
            return;
        }

        //set part of ack values
        ack_ptr->user_name_ = auth_req_ptr->srv_auth_req.user_name_;
        ack_ptr->token_ = auth_req_ptr->srv_auth_req.token_;

        //check the web client is legal to access or not
        if ((http_srv_name_ != auth_req_ptr->http_server_name_) || 
            (http_auth_pwd_ != auth_req_ptr->http_auth_pwd_)) {
            ack_ptr->result_code_ = http_server_verify_no_right_access_code;
            ack_ptr->result_msg_ = http_server_verify_no_right_access;
            rep = reply::stock_reply(reply::unauthorized, ack_ptr);
            return;
        }

        //verify user name
        user_info u_info;
        if (get_server_cache()->get_user_info(auth_req_ptr->srv_auth_req.user_name_, u_info)) {
            //verify user token
            token_info tkn_info;
            ret = get_server_cache()->get_token_info(auth_req_ptr->srv_auth_req.token_, tkn_info);
            if (ret) {
                ack_ptr->result_code_ = http_server_verify_success_code;
                ack_ptr->result_msg_ = http_server_verify_success;
                ack_ptr->server_sys_time_ = server_utils::get_current_system_time();
                ack_ptr->user_id_ = u_info.user_id_;
            }
            else {
                ack_ptr->result_code_ = http_server_verify_fail_code;
                ack_ptr->result_msg_ = http_server_verify_fail;
            }
        }
        else {
            ack_ptr->result_code_ = http_server_verify_invalid_user_code;
            ack_ptr->result_msg_ = http_server_verify_invalid_user;
        }
        rep = reply::stock_reply(reply::ok, ack_ptr);
    }
    else {
        ack_ptr->result_code_ = http_server_verify_req_type_fail_code;
        ack_ptr->result_msg_ = http_server_verify_req_type_fail;
        rep = reply::stock_reply(reply::bad_request, ack_ptr);
        log_warn(http_request_handler_module, "undefined request type:%s.", req_type.c_str());
    }
}

bool request_handler::url_decode(const std::string& in, std::string& out) {
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else if (in[i] == '+') {
            out += ' ';
        }
        else {
            out += in[i];
        }
    }
    return true;
}

bool request_handler::parse_json_tk_vfy_req(const std::string &json_content, 
    HttpAuthReqPtr tk_vfy_req) {
    if (nullptr == tk_vfy_req)
        return false;

    cJSON *root = cJSON_Parse(json_content.c_str());
    if (!root) {
        log_warn(http_request_handler_module, "[parse_json_tk_vfy_req]:parse json root failed!");
        return false;
    }

    cJSON *item = cJSON_GetObjectItem(root, "http_server_name");
    if (!item) {
        log_warn(http_request_handler_module, "[parse_json_tk_vfy_req]:parse json http_server_name failed!");
        cJSON_Delete(root);
        return false;
    }
    tk_vfy_req->http_server_name_ = item->valuestring;

    item = cJSON_GetObjectItem(root, "http_auth_pwd");
    if (!item) {
        log_warn(http_request_handler_module, "[parse_json_tk_vfy_req]:parse json http_auth_pwd failed!");
        cJSON_Delete(root);
        return false;
    }
    tk_vfy_req->http_auth_pwd_ = item->valuestring;

    item = cJSON_GetObjectItem(root, "user_name");
    if (!item) {
        log_warn(http_request_handler_module, "[parse_json_tk_vfy_req]:parse json user_name failed!");
        cJSON_Delete(root);
        return false;
    }
    tk_vfy_req->srv_auth_req.user_name_ = item->valuestring;

    item = cJSON_GetObjectItem(root, "token");
    if (!item) {
        log_warn(http_request_handler_module, "[parse_json_tk_vfy_req]:parse json token failed!");
        cJSON_Delete(root);
        return false;
    }
    tk_vfy_req->srv_auth_req.token_ = item->valuestring;

    cJSON_Delete(root);
    return true;
}

} //namespace server
} //namespace http
