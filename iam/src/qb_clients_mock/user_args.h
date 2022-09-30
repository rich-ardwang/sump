#pragma once
#include <string>
#include <stdint.h>

struct user_args_type {
    user_args_type() : io_threads(2), work_threads(1), use_cluster(false), 
                       test_mode(false), req_speed(0), type(0) {}
    int             io_threads;
    int             work_threads;
    bool            use_cluster;
    std::string     server_addr;
    std::string     port;
    std::string     cluster_addr;
    std::string     name;
    bool            test_mode;
    uint32_t        req_speed;
    int             type;
};
