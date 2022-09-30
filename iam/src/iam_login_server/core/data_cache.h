/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : data_cache.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-11
*Desc        : design data cache class for login server,
*            : this class take charge for maintaining all kinds of data of the server,
*            : such as token, user information and so on.
*History     : Add by Lei.Wang 2019-10-11
*              Modify by Lei.Wang 2019-10-18
***************************************************************************************************/
#ifndef __DATA_CACHE_H__
#define __DATA_CACHE_H__
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "rwlock.h"
#include "lava_utils_api.h"
#include "common.h"

#define data_cache_module      "iam.login_server.data_cache"

namespace iam {
namespace login_server {

//------------------data definition------------------------//
//QB clients authentication requirements
typedef struct tag_client_auth_req {
    tag_client_auth_req() : user_name_(""), user_pwd_(""), 
                            cli_mac_addr_(""), cli_version_("") {}

    std::string     user_name_;
    std::string     user_pwd_;
    std::string     cli_mac_addr_;
    std::string     cli_version_;
} client_auth_req;

//login server response the authentication result to QB clients
typedef struct tag_client_auth_ack {
    tag_client_auth_ack() : user_name_(""), user_id_(""), 
                            gtw_srv_addr_(""), gtw_srv_port_(""), 
                            dds_srv_addr_(""), dds_srv_port_(""), 
                            token_(""), server_sys_time_(""), 
                            result_code_(-1), result_msg_("") {}

    std::string     user_name_;
    std::string     user_id_;
    std::string     gtw_srv_addr_;
    std::string     gtw_srv_port_;
    std::string     dds_srv_addr_;
    std::string     dds_srv_port_;
    std::string     token_;
    std::string     server_sys_time_;
    int             result_code_;
    std::string     result_msg_;
} client_auth_ack;

//other servers authentication requirements
typedef struct tag_server_auth_req {
    tag_server_auth_req() : user_name_(""), token_("") {}

    std::string     user_name_;
    std::string     token_;
} server_auth_req;

//login server response the authentication result to other servers
typedef struct tag_server_auth_ack {
    tag_server_auth_ack() : user_name_(), user_id_(), 
                            token_(), server_sys_time_(), 
                            result_code_(-1), result_msg_("") {}

    std::string     user_name_;
    std::string     user_id_;
    std::string     token_;
    std::string     server_sys_time_;
    int             result_code_;
    std::string     result_msg_;
} server_auth_ack;

//user_info come from QB database
typedef struct tag_user_info {
    tag_user_info() : opt_flag_(0), user_name_(""), user_pwd_(""), 
                      user_id_(""), cli_mac_addr_(""), cli_version_("") {}

    short           opt_flag_;
    std::string     user_name_;
    std::string     user_pwd_;
    std::string     user_id_;
    std::string     cli_mac_addr_;
    std::string     cli_version_;
} user_info;

//token_info commonly used to store the tokens' information 
//and also used to verify tokens or sync tokens.
typedef struct tag_token_info {
    tag_token_info() : token_(""), token_time_(0) {}

    std::string     token_;
    int64_t         token_time_;  //record the timestamp of creating token
    std::string     user_name_;   //the map for token and user info
} token_info;

typedef std::map<std::string, token_info>       TokenInfo;
typedef std::pair<std::string, token_info>      TokenInfoPair;
typedef std::map<std::string, user_info>        UserInfo;
typedef std::pair<std::string, user_info>       UserInfoPair;
typedef std::map<std::string, std::string>      UserNameTokenMap;
typedef std::pair<std::string, std::string>     UserNameTokenMapPair;
typedef std::shared_ptr<client_auth_req>        ClientAuthReqPtr;
typedef std::shared_ptr<client_auth_ack>        ClientAuthAckPtr;
typedef std::shared_ptr<server_auth_req>        ServerAuthReqPtr;
typedef std::shared_ptr<server_auth_ack>        ServerAuthAckPtr;

//interface of server cache
struct i_srv_cache {
    virtual bool generate_token(__lv_in const user_info &u_info, 
        __lv_out token_info &u_token_info) = 0;
    virtual void add_token_info(__lv_in const token_info &u_token_info) = 0;
    virtual void remove_all_invalid_token_info() = 0;
    virtual bool get_token_info(__lv_in const std::string &u_token, 
        __lv_out token_info &u_token_info) = 0;
    virtual bool get_token_by_uname(__lv_in const std::string &user_name, 
        __lv_out std::string &token) = 0;
    virtual void write_user_info(__lv_in const std::vector<user_info> &vec_usr_info) = 0;
    virtual void update_user_info(__lv_in const user_info &u_info, 
        __lv_in const int &opt) = 0;
    virtual bool get_user_info(__lv_in const std::string &u_name, 
        __lv_out user_info &u_info) = 0;
};

//------------------class definition------------------------//
class server_cache : public i_srv_cache {
public:
    server_cache();
    ~server_cache();

public:
    virtual bool generate_token(__lv_in const user_info &u_info, 
        __lv_out token_info &u_token_info) override;
    virtual void add_token_info(__lv_in const token_info &u_token_info) override;
    virtual void remove_all_invalid_token_info() override;
    virtual bool get_token_info(__lv_in const std::string &u_token, 
        __lv_out token_info &u_token_info) override;
    virtual bool get_token_by_uname(__lv_in const std::string &user_name, 
        __lv_out std::string &token) override;
    virtual void write_user_info(__lv_in const std::vector<user_info> &vec_usr_info) override;
    virtual void update_user_info(__lv_in const user_info &u_info, 
        __lv_in const int &opt) override;
    virtual bool get_user_info(__lv_in const std::string &u_name, 
        __lv_out user_info &u_info) override;

private:
    void data_init();
    bool check_token_info(__lv_in const token_info &u_token_info);
    bool check_user_info(__lv_in const user_info &u_info);
    void write_token_info(__lv_in const std::vector<token_info> &vec_token_info);
    void add_un_tk_map(__lv_in const token_info &u_token_info);

private:
    TokenInfo           *token_info_;
    UserInfo            *user_info_;
    UserNameTokenMap    *u_name_token_map_;
    lava::RWLock4       token_info_lock_;
    lava::RWLock4       user_info_lock_;
    lava::RWLock4       u_name_token_map_lock_;
};

}
}

#endif /* __DATA_CACHE_H__ */
