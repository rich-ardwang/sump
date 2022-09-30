#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "wsa.h"

// utils
// show deal info
void printDealInfo(void *tid);
// show collateral flow
void printAverageCollatFlow(void* tid);
// show bond flow
void printAverageBondFlow(void* tid);


#define CHECK_API_RETURN_VALUE(tid, expect_value, value) \
{ \
    int ret = value; \
    if (ret != expect_value) \
    { \
        std::cerr << get_deal_error_msg(tid) << std::endl; \
        break; \
    } \
}
