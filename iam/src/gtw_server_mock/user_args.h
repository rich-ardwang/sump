#pragma once
#include <string>
#include <stdint.h>

struct user_args_type {
    user_args_type() : server_addr(""), send_queue(""), listen_queue(""), 
                       test_mode(false), req_speed(0) {}
    std::string     server_addr;
    std::string     send_queue;
    std::string     listen_queue;
    bool            test_mode;
    uint32_t        req_speed;
};
