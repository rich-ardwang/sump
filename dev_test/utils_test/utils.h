/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : utils.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-12
*Desc        : common functions and utils for login server.
*History     : Add by Lei.Wang 2019-10-12
***************************************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__
#pragma once
#include <string>

#define data_time_format       "%d-%02d-%02d %02d:%02d:%02d"
#define time_format            "%02d:%02d:%02d"
#define data_format1           "%d-%02d-%02d"
#define data_format2           "%d/%02d/%02d"


class CUtils {
public:
    CUtils() {}
    ~CUtils() {}

    static std::string getCurrentSystemTime(const std::string format = data_time_format);
};

#endif /* __COMMON_H__ */
