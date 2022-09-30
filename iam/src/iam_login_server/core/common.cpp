#include "common.h"


namespace iam {
namespace login_server {

int server_utils::get_process_id() {
#ifdef LAVA_WIN
    return static_cast<int>(_getpid());
#else
    return static_cast<int>(getpid());
#endif
}

double server_utils::year2month(const double src) {
    return src * 12.0;
}

double server_utils::month2year(const double src) {
    return src / 12.0;
}

std::string server_utils::get_current_system_time(const char *format 
    /* = data_time_format */) {
    auto tt = std::chrono::system_clock::to_time_t
        (std::chrono::system_clock::now());
    struct tm *ptm = new tm();
#ifdef LAVA_WIN
    localtime_s(ptm, &tt);
#else
	localtime_r(&tt, ptm);
#endif
    char date[30] = { 0 };
    if (time_format == format)
#ifdef LAVA_WIN
        sprintf_s(date, format, (int)ptm->tm_hour, 
        (int)ptm->tm_min, (int)ptm->tm_sec);
#else
		snprintf(date, sizeof(date), format, (int)ptm->tm_hour, 
        (int)ptm->tm_min, (int)ptm->tm_sec);
#endif
    else
#ifdef LAVA_WIN
        sprintf_s(date, format, (int)ptm->tm_year + 1900, 
        (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, 
        (int)ptm->tm_min, (int)ptm->tm_sec);
#else
		snprintf(date, sizeof(date), format, (int)ptm->tm_year + 1900, 
        (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour, 
        (int)ptm->tm_min, (int)ptm->tm_sec);
#endif
    if (ptm) {
        delete ptm;
        ptm = nullptr;
    }
    return std::string(date);
}

int64_t server_utils::get_current_system_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(epoch);
    return timestamp.count();
}

void server_utils::split(const std::string& strLine, const char cSplit, 
    std::vector<std::string>& vecContent) {
    //single field
    if (strLine.find_first_of(cSplit, 0) == std::string::npos)
        vecContent.push_back(strLine);
    //multi fields
    else {
        //get the fields
        std::string::size_type lastSplit = strLine.find_first_of(cSplit, 0);
        vecContent.push_back(strLine.substr(0, lastSplit));

        std::string::size_type curSplit = strLine.find_first_of(cSplit, lastSplit + 1);
        while (std::string::npos != curSplit) {
            vecContent.push_back(strLine.substr(lastSplit + 1, curSplit - lastSplit - 1));
            lastSplit = curSplit;
            curSplit = strLine.find_first_of(cSplit, lastSplit + 1);
        }

        //get the last field
        std::string strLastValue = strLine.substr(lastSplit + 1, strLine.size() - lastSplit);
        vecContent.push_back(strLastValue);
    }
}

}
}
