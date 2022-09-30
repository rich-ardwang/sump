/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_reply.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : take charge of http replying.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_REPLY_H__
#define __HTTP_REPLY_H__
#include <string>
#include <vector>
#include <asio.hpp>
#include "http_header.h"
#include "data_cache.h"


namespace http {
namespace server {

typedef std::shared_ptr<iam::login_server::server_auth_ack>        HttpAuthAckPtr;

//a reply to be sent to a client.
struct reply {
    //the status of the reply.
    enum status_type {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        no_data = 205,
        multiple_choices = 300,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implement = 501,
        bad_gateway = 502,
        service_unavailable = 503
    } status;

    //the headers to be included in the reply.
    std::vector<header> headers;

    //the content to be sent in the reply.
    std::string content;

    //convert the reply into a vector of buffers. The buffers do not own the
    //underlying memory blocks, therefore the reply object must remain valid and
    //not be changed until the write operation has completed.
    std::vector<asio::const_buffer> to_buffers();

    //get a stock reply.
    static reply stock_reply(status_type status, HttpAuthAckPtr auth_ack_ptr);
};

} //namespace server
} //namespace http

#endif /* __HTTP_REPLY_H__ */
