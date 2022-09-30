/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_connection.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : the definition of http connection class.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_CONNECTION_H__
#define __HTTP_CONNECTION_H__
#include <array>
#include <memory>
#include <asio.hpp>
#include "http_reply.h"
#include "http_request.h"
#include "http_request_parser.h"
#include "http_request_handler.h"


namespace http {
namespace server {

class connection_manager;

//represents a single connection from a client.
class connection : public std::enable_shared_from_this<connection> {
public:
    connection(const connection&) = delete;
    connection& operator=(const connection&) = delete;

    //construct a connection with the given socket.
    explicit connection(asio::ip::tcp::socket socket, 
        connection_manager& manager, request_handler& handler);

    //start the first asynchronous operation for the connection.
    void start();

    //stop all asynchronous operations associated with the connection.
    void stop();

private:
    //perform an asynchronous read operation.
    void do_read();

    //perform an asynchronous write operation.
    void do_write();

    //socket for the connection.
    asio::ip::tcp::socket socket_;

    //the manager for this connection.
    connection_manager& connection_manager_;

    //the handler used to process the incoming request.
    request_handler& request_handler_;

    //buffer for incoming data.
    std::array<char, 8192> buffer_;

    //the incoming request.
    request request_;

    //the parser for the incoming request.
    request_parser request_parser_;

    //the reply to be sent back to the client.
    reply reply_;
};

typedef std::shared_ptr<connection> connection_ptr;

} //namespace server
} //namespace http

#endif /* __HTTP_CONNECTION_H__ */
