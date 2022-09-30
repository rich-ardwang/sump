#pragma once
#include <Windows.h>

#define MAP_VIEW_BUF_SIZE 130

//token info
struct trans_token_info {
    char       u_name[65];
    char       token[65];
};

extern void parse_token_info(trans_token_info &tk_info);
