#include "singleton.h"
#include "data_cache.h"
#include "lava_utils_api.h"

//need extern using out of this file
iam::login_server::i_srv_cache *get_server_cache() {
    return singleton_lazy<iam::login_server::server_cache, 
        iam::login_server::i_srv_cache>::instance();
}

void release_server_cache() {
    singleton_lazy<iam::login_server::server_cache, 
        iam::login_server::i_srv_cache>::release();
}

#define SHA256_OUTPUT_LENGTH        (32 * 2 + 1)


namespace iam {
namespace login_server {

//------------------class member function impl------------------------//
server_cache::server_cache() : token_info_(nullptr), user_info_(nullptr), 
    u_name_token_map_(nullptr) {
    data_init();
}

server_cache::~server_cache() {
    if (token_info_) {
        token_info_->clear();
        delete token_info_;
        token_info_ = nullptr;
    }
    if (user_info_) {
        user_info_->clear();
        delete user_info_;
        user_info_ = nullptr;
    }
    if (u_name_token_map_) {
        u_name_token_map_->clear();
        delete u_name_token_map_;
        u_name_token_map_ = nullptr;
    }
}

bool server_cache::generate_token(__lv_in const user_info &u_info, 
    __lv_out token_info &u_token_info) {
    //check input
    if (!check_user_info(u_info)) {
        log_warn(data_cache_module, "the name, id or password in user info must not be empty!");
        log_debug(data_cache_module, "user_name=%s, user_id=%s, user_pwd=%s.", 
            u_info.user_name_.c_str(), u_info.user_id_.c_str(), 
            u_info.user_pwd_.c_str());
        return false;
    }

    //create sha256 input data and generate sha256 digest message
    std::string sha256_in;
    sha256_in = u_info.user_name_;
    sha256_in += u_info.user_pwd_;
    sha256_in += u_info.cli_mac_addr_;
    sha256_in += u_info.cli_version_;
    int64_t tk_time = server_utils::get_current_system_timestamp();
    sha256_in += std::to_string(tk_time);
    char result_buf[SHA256_OUTPUT_LENGTH];
    int ret = sha256_encode(sha256_in.c_str(), sha256_in.length(), 
        result_buf, SHA256_OUTPUT_LENGTH);
    if (0 != ret) {
        log_warn(data_cache_module, "sha256 encoding failed!");
        return false;
    }
    std::string sha256_out(result_buf);

    //set return data
    u_token_info.token_time_ = tk_time;
    u_token_info.token_ = sha256_out;
    u_token_info.user_name_ = u_info.user_name_;

    return true;
}

void server_cache::add_token_info(__lv_in const token_info &u_token_info) {
    //check input
    if (!check_token_info(u_token_info)) {
        log_warn(data_cache_module, "the fields of u_token_info must not be empty!");
#ifdef WIN32    //just compile as 32 bit
        int tk_time = static_cast<int>(u_token_info.token_time_);
#else           //compile as 64 bit
        int64_t tk_time = u_token_info.token_time_;
#endif
        log_debug(data_cache_module, "token_=%s, token_time_=%lld, user_name=%s.", 
            u_token_info.token_.c_str(), tk_time, u_token_info.user_name_.c_str());
        return;
    }

    //user name and token map store
    add_un_tk_map(u_token_info);

    //user info store
    token_info_lock_.lock_r();
    auto it = token_info_->find(u_token_info.token_);
    if (it == token_info_->end()) {
        token_info_lock_.unlock_r();
        token_info_lock_.lock_w();
        token_info_->insert(TokenInfoPair(u_token_info.token_, u_token_info));
        token_info_lock_.unlock_w();
        return;
    }
    token_info_lock_.unlock_r();
}

void server_cache::remove_all_invalid_token_info() {
    if (token_info_->empty())
        return;

    //get token ttl, unit: second
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("loginsrv.token.ttl", conf_buf, conf_len);
    std::string ttl(conf_buf);
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }
    int64_t tkn_ttl = 60 * 60;  //default token ttl is one hour
    if (!ttl.empty())
        tkn_ttl = std::stoll(ttl);

    //filter invalid tokens
    int64_t now_tt = server_utils::get_current_system_timestamp();
    std::vector<token_info> vec_tkn_info;
    token_info_lock_.lock_r();
    auto it = token_info_->begin();
    for (; it != token_info_->end(); it++) {
        if ((now_tt - it->second.token_time_) <= tkn_ttl)
            vec_tkn_info.push_back(it->second);
    }
    token_info_lock_.unlock_r();

    //write new tokens and the map
    if (!vec_tkn_info.empty())
        write_token_info(vec_tkn_info);
}

bool server_cache::get_token_info(__lv_in const std::string &u_token, 
    __lv_out token_info &u_token_info) {
    //check input
    if (u_token.empty())
        return false;

    //query token info
    token_info_lock_.lock_r();
    auto it = token_info_->find(u_token);
    if (it != token_info_->end()) {
        u_token_info = it->second;
        token_info_lock_.unlock_r();
        return true;
    }
    token_info_lock_.unlock_r();

    return false;
}

bool server_cache::get_token_by_uname(__lv_in const std::string &user_name, 
    __lv_out std::string &token) {
    //check input
    if (user_name.empty())
        return false;

    //query token by user name
    u_name_token_map_lock_.lock_r();
    auto it = u_name_token_map_->find(user_name);
    if (it != u_name_token_map_->end()) {
        token = it->second;
        u_name_token_map_lock_.unlock_r();
        return true;
    }
    u_name_token_map_lock_.unlock_r();

    return false;
}

void server_cache::write_user_info(__lv_in const std::vector<user_info> &vec_usr_info) {
    //check input
    if (vec_usr_info.empty())
        return;

    //write user info
    UserInfo *new_user_info_ = new UserInfo;
    for (auto &item : vec_usr_info) {
        if (!check_user_info(item)) {
            log_warn(data_cache_module, "the fields of user_info must not be empty!");
            log_debug(data_cache_module, "user_name=%s, user_id=%s, user_pwd=%s, cli_mac_addr=%s, cli_version_=%s.", 
                item.user_name_.c_str(), item.user_id_.c_str(), 
                item.user_pwd_.c_str(), item.cli_mac_addr_.c_str(), 
                item.cli_version_.c_str());
            continue;
        }
        new_user_info_->insert(UserInfoPair(item.user_name_, item));
    }
    if (!new_user_info_->empty()) {
        UserInfo *old_user_info_ = user_info_;
        user_info_lock_.lock_w();
        user_info_ = new_user_info_;
        user_info_lock_.unlock_w();
        if (old_user_info_) {
            delete old_user_info_;
            old_user_info_ = nullptr;
        }
    }
}

void server_cache::update_user_info(__lv_in const user_info &u_info, __lv_in const int &opt) {
    //check input
    if (!check_user_info(u_info)) {
        log_warn(data_cache_module, "the fields of user_info must not be empty!");
        log_debug(data_cache_module, "user_name=%s, user_id=%s, user_pwd=%s, cli_mac_addr=%s, cli_version_=%s.", 
            u_info.user_name_.c_str(), u_info.user_id_.c_str(), 
            u_info.user_pwd_.c_str(), u_info.cli_mac_addr_.c_str(), 
            u_info.cli_version_.c_str());
        return;
    }

    //update user info
    user_info_lock_.lock_w();
    auto it = user_info_->find(u_info.user_name_);
    if (it != user_info_->end()) {
        if (0 == opt) //update item
            it->second = u_info;
        else if (2 == opt) //delete item
            it = user_info_->erase(it);
    }
    else {
        if (1 == opt) //insert user info
            user_info_->insert(UserInfoPair(u_info.user_name_, u_info));
    }
    user_info_lock_.unlock_w();
}

bool server_cache::get_user_info(__lv_in const std::string &u_name, 
    __lv_out user_info &u_info) {
    //check input
    if (u_name.empty())
        return false;

    //query user info
    user_info_lock_.lock_r();
    auto it = user_info_->find(u_name);
    if (it != user_info_->end()) {
        u_info = it->second;
        user_info_lock_.unlock_r();
        return true;
    }
    user_info_lock_.unlock_r();

    return false;
}

void server_cache::data_init() {
    token_info_ = new TokenInfo;
    user_info_ = new UserInfo;
    u_name_token_map_ = new UserNameTokenMap;
}

bool server_cache::check_token_info(__lv_in const token_info &u_token_info) {
    bool ret = true;
    if (u_token_info.token_.empty() || u_token_info.user_name_.empty() 
        || (u_token_info.token_time_ <= 0))
        ret = false;
    return ret;
}

bool server_cache::check_user_info(__lv_in const user_info &u_info) {
    bool ret = true;
    if (u_info.user_name_.empty() || u_info.user_pwd_.empty() ||
        u_info.user_id_.empty())
        ret = false;
    return ret;
}

void server_cache::write_token_info(__lv_in const std::vector<token_info> &vec_token_info) {
    //check input
    if (vec_token_info.empty())
        return;

    //write user info
    TokenInfo *new_token_info_ = new TokenInfo;
    UserNameTokenMap *new_map_ = new UserNameTokenMap;
    for (auto &item : vec_token_info) {
        if (!check_token_info(item)) {
            log_warn(data_cache_module, "the fields of token_info must not be empty!");
#ifdef WIN32
            int tk_time = static_cast<int>(item.token_time_);
#else
            int64_t tk_time = item.token_time_;
#endif
            log_debug(data_cache_module, "token_=%s, token_time_=%lld, user_name=%s.", 
                item.token_.c_str(), tk_time, item.user_name_.c_str());
            continue;
        }
        new_token_info_->insert(TokenInfoPair(item.token_, item));
        new_map_->insert(UserNameTokenMapPair(item.user_name_, item.token_));
    }
    if (!new_token_info_->empty()) {
        TokenInfo *old_token_info_ = token_info_;
        user_info_lock_.lock_w();
        token_info_ = new_token_info_;
        user_info_lock_.unlock_w();
        if (old_token_info_) {
            delete old_token_info_;
            old_token_info_ = nullptr;
        }
    }
    if (!new_map_->empty()) {
        UserNameTokenMap *old_map_ = u_name_token_map_;
        u_name_token_map_lock_.lock_w();
        u_name_token_map_ = new_map_;
        u_name_token_map_lock_.unlock_w();
        if (old_map_) {
            delete old_map_;
            old_map_ = nullptr;
        }
    }
}

void server_cache::add_un_tk_map(__lv_in const token_info &u_token_info) {
    u_name_token_map_lock_.lock_r();
    auto it = u_name_token_map_->find(u_token_info.user_name_);
    if (it == u_name_token_map_->end()) {
        u_name_token_map_lock_.unlock_r();
        u_name_token_map_lock_.lock_w();
        u_name_token_map_->insert(UserNameTokenMapPair(u_token_info.user_name_, 
            u_token_info.token_));
        u_name_token_map_lock_.unlock_w();
        return;
    }
    u_name_token_map_lock_.unlock_r();
}

}
}
