#include <chrono>
#include "utils.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string CUtils::getCurrentSystemTime(const std::string format /* = data_time_format */) {
    auto tt = std::chrono::system_clock::to_time_t
        (std::chrono::system_clock::now());
    struct tm *ptm = new tm();
    localtime_s(ptm, &tt);
    char date[30] = { 0 };
    if (time_format == format)
        sprintf_s(date, format.c_str(), (int)ptm->tm_hour, 
            (int)ptm->tm_min, (int)ptm->tm_sec);
    else
        sprintf_s(date, format.c_str(), (int)ptm->tm_year + 1900, 
            (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, 
            (int)ptm->tm_min, (int)ptm->tm_sec);
    if (ptm) {
        delete ptm;
        ptm = nullptr;
    }
    return std::string(date);
}

