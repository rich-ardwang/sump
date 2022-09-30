/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : common.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-17
*Desc        : common functions and utils for login server.
*History     : Add by Lei.Wang 2019-09-17
*              Modify by Lei.Wang 2019-10-12
***************************************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__
#include <chrono>
#include <string>
#include <vector>

#ifdef LAVA_WIN
#include <process.h>
#else
#include <unistd.h>
#endif

//work for get_current_system_time function
#define data_time_format       "%d-%02d-%02d %02d:%02d:%02d"
#define time_format            "%02d:%02d:%02d"
#define data_format1           "%d-%02d-%02d"
#define data_format2           "%d/%02d/%02d"


namespace iam {
namespace login_server {

typedef std::chrono::system_clock::time_point   TimePoint;

class server_utils {
public:
    server_utils() {}
    ~server_utils() {}

    //get process id
    static int get_process_id();

    //year convert to month
    static double year2month(const double src);

    //month convert to year
    static double month2year(const double src);

    //get current system time to string
    //the return value is time of string format
    static std::string get_current_system_time(const char *format = data_time_format);

    //get current system time to timestamp
    //the return value is timestamp, unit: second
    static int64_t get_current_system_timestamp();

    //split the string to vector
    static void split(const std::string& strLine, const char cSplit, 
        std::vector<std::string>& vecContent);
};

}
}

#endif /* __COMMON_H__ */
