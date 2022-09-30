#pragma once
#include <string>
#include <stdint.h>

struct user_args_type {
    user_args_type() : server_addr(""), create_user_info_cnt(100) {}
    std::string     server_addr;
    int             create_user_info_cnt;
};
