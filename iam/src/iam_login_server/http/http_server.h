/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_server.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : the implementation of http server.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__
#include <thread>
#include <memory>
#include <string>
#include <asio.hpp>
#include "http_connection.h"
#include "http_connection_manager.h"
#include "http_request_handler.h"

#define http_server_moduler        "iam.login_server.http_server"


namespace iam {
namespace login_server {

//interfaces of http server
struct i_http_server {
    virtual bool start() = 0;
    virtual bool stop() = 0;
};

class server_impl;
//http server class used external
class http_server : public i_http_server {
public:
    http_server();
    ~http_server();

public:
    virtual bool start() override;
    virtual bool stop() override;

private:
    //basic http server info
    std::string http_srv_ip_;
    std::string http_srv_port_;
    std::string http_root_path_;
    std::string http_srv_name_;
    std::string http_auth_pwd_;

    //http server object
    server_impl *server_impl_;

    //http worker thread
    std::shared_ptr<std::thread> worker_thread_;
};

//the implementation class of the HTTP server.
class server_impl {
public:
    server_impl(const server_impl&) = delete;
    server_impl& operator=(const server_impl&) = delete;

    //construct the server to listen on the specified TCP address and port, and
    //serve up files from the given directory.
    explicit server_impl(const std::string& address, const std::string& port, 
        const std::string& doc_root, const std::string &srv_name, const std::string &auth_pwd);

    //run the server's io_context loop.
    void run();

    //control http server in external functions
    void stop();

private:
    //perform an asynchronous accept operation.
    void do_accept();

    //wait for a request to stop the server.
    void do_await_stop();

    //the io_context used to perform asynchronous operations.
    asio::io_context io_context_;

    //the signal_set is used to register for process termination notifications.
    asio::signal_set signals_;

    //acceptor used to listen for incoming connections.
    asio::ip::tcp::acceptor acceptor_;

    //the connection manager which owns all live connections.
    http::server::connection_manager connection_manager_;

    //the handler for all incoming requests.
    http::server::request_handler request_handler_;
};

} //namespace server
} //namespace http

#endif /* __HTTP_SERVER_H__ */
