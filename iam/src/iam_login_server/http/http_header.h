/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_header.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : the definition of http header.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_HEADER_H__
#define __HTTP_HEADER_H__
#include <string>


namespace http {
namespace server {

struct header {
    std::string name;
    std::string value;
};

} //namespace server
} //namespace http

#endif /* __HTTP_HEADER_H__ */
