/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : server_msg_def.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-18
*Desc        : we define all messages here which used in login_server internally.
*History     : Add by Lei.Wang 2019-10-18
***************************************************************************************************/
#ifndef __SERVER_MESSAGE_DEF_H__
#define __SERVER_MESSAGE_DEF_H__

#define parse_message_separator                  ','

/* lava message for query */
#define lava_msg_body_identity_flg               4   //[INT] 0: server, 1: client
#define lava_msg_body_function_id                5   //[INT] 1: user_login, 2:server_verify
#define lava_msg_body_query_data                 6   //[STRING] query data

/* lava message for response */
#define lava_msg_body_handle_ret_code            4   //[INT]
#define lava_msg_body_handle_ret_msg             5   //[STRING]
#define lava_msg_body_ack_data                   6   //[STRING]

/* login server handle message */
#define login_server_handle_fail_code            -3
#define login_server_handle_fail                 "login server handle failed!"

/* lava business handle message */
#define lava_user_login_success_code             0
#define lava_user_login_name_fail_code           -1
#define lava_user_login_pwd_fail_code            -2
#define lava_server_verify_sucess_code           0
#define lava_server_verify_fail_code             -1
#define lava_server_verify_invalid_user_code     -2
#define lava_kick_msg_topic                      "iam.login_server.kick_client"
#define lava_user_login_success                  "login success."
#define lava_user_login_name_fail                "user name not exist!"
#define lava_user_login_pwd_fail                 "password not correct!"
#define lava_server_verify_success               "token passed."
#define lava_server_verify_fail                  "token not passed!"
#define lava_server_verify_invalid_user          "invalid user name!"

/* msgbus business handle message */
#define msgbus_server_verify_sucess_code         0
#define msgbus_server_verify_fail_code           -1
#define msgbus_server_verify_invalid_user_code   -2
#define msgbus_server_verify_success             "token passed."
#define msgbus_server_verify_fail                "token not passed!"
#define msgbus_server_verify_invalid_user        "invalid user name!"

/* http business handle def */
#define http_server_verify_success_code          0
#define http_server_verify_fail_code             -1
#define http_server_verify_invalid_user_code     -2
#define http_server_verify_parse_url_fail_code   -3
#define http_server_verify_req_type_fail_code    -4
#define http_server_verify_req_param_fail_code   -5
#define http_server_verify_no_right_access_code  -6
#define http_request_type                        "login-auth"
#define http_server_verify_success               "token passed."
#define http_server_verify_fail                  "token not passed!"
#define http_server_verify_invalid_user          "invalid user name!"
#define http_server_verify_parse_url_fail        "parse url failed!"
#define http_server_verify_req_type_fail         "request type not correct!"
#define http_server_verify_req_param_fail        "request parameters not correct!"
#define http_server_verify_no_right_access       "have no right to access http server!"

#endif  /* __SERVER_MESSAGE_DEF_H__ */
