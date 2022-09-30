
#include "wsa.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

// utils
// show deal info
void printDealInfo(void *tid);

// show the first num_periods collateral flow, pass -1 for num_periods to show all the cashflow
void printCollatFlow(void* tid, int num_periods);

// show the first num_periods bond flow for each bond, pass -1 for num_periods to show all the cashflow
void printBondFlow(void* tid, int num_periods);


#define CHECK_API_RETURN_VALUE(tid, expect_value, value) \
{ \
    int ret = value; \
    if (ret != expect_value) \
    { \
        std::cerr << get_deal_error_msg(tid) << std::endl; \
        break; \
    } \
}
