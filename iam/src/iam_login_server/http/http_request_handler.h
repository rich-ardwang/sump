/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_request_handler.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : the handler of http request.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_REQUEST_HANDLER_H__
#define __HTTP_REQUEST_HANDLER_H__
#include <string>
#include "http_request.h"

#define http_request_handler_module      "iam.login_server.http_request_handler"


namespace http {
namespace server {

struct reply;
struct request;

//the common handler for all incoming requests.
class request_handler {
public:
    request_handler(const request_handler&) = delete;
    request_handler& operator=(const request_handler&) = delete;

    //construct with a directory containing files to be served.
    explicit request_handler(const std::string &root_path, const std::string &srv_name, 
        const std::string &auth_pwd);

    //handle a request and produce a reply.
    void handle_request(const request& req, reply& rep);

private:
    //perform URL-decoding on a string. Returns false if the encoding was invalid.
    static bool url_decode(const std::string& in, std::string& out);

    //parse token verify req from json format
    static bool parse_json_tk_vfy_req(const std::string &json_content, 
        HttpAuthReqPtr tk_vfy_req);

private:
    //the root path of http server
    std::string root_path_;

    //http server name
    std::string http_srv_name_;

    //http server auth password
    std::string http_auth_pwd_;
};

} //namespace server
} //namespace http

#endif /* __HTTP_REQUEST_HANDLER_H__ */
