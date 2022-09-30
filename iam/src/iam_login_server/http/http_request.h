/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_request.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : take charge of http requesting.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__
#include <string>
#include <vector>
#include "http_header.h"
#include "data_cache.h"

using namespace iam::login_server;


namespace http {
namespace server {

//a request received from a client.
struct request {
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<header> headers;
    std::string content;
};

typedef struct tag_token_auth_req {
    tag_token_auth_req() : http_server_name_(""), http_auth_pwd_("") {}

    std::string         http_server_name_;
    std::string         http_auth_pwd_;
    server_auth_req     srv_auth_req;
} token_auth_req;

typedef std::shared_ptr<token_auth_req>        HttpAuthReqPtr;

} //namespace server
} //namespace http

#endif /* __HTTP_REQUEST_H__ */
