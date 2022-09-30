#include <signal.h>
#include <utility>
#include "http_server.h"
#include "singleton.h"
#include "lava_utils_api.h"

//need extern using out of this file
iam::login_server::i_http_server *get_http_server() {
    return singleton_lazy<iam::login_server::http_server, 
        iam::login_server::i_http_server>::instance();
}

void release_http_server() {
    singleton_lazy<iam::login_server::http_server, 
        iam::login_server::i_http_server>::release();
}


namespace iam {
namespace login_server {

http_server::http_server() {
    //read config for http server
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("http.server.addr", conf_buf, conf_len);
    http_srv_ip_ = conf_buf;
    get_lava_config()->value("http.server.port", conf_buf, conf_len);
    http_srv_port_ = conf_buf;
    get_lava_config()->value("http.server.root_path", conf_buf, conf_len);
    http_root_path_ = conf_buf;
    get_lava_config()->value("http.server.name", conf_buf, conf_len);
    http_srv_name_ = conf_buf;
    get_lava_config()->value("http.server.pwd", conf_buf, conf_len);
    http_auth_pwd_ = conf_buf;
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }
    assert(!http_srv_ip_.empty());
    assert(!http_srv_port_.empty());
    assert(!http_root_path_.empty());
    assert(!http_srv_name_.empty());
    assert(!http_auth_pwd_.empty());

    //create object of the server
    server_impl_ = new server_impl(http_srv_ip_, http_srv_port_, http_root_path_, 
        http_srv_name_, http_auth_pwd_);
}

http_server::~http_server() {
    if (server_impl_) {
        delete server_impl_;
        server_impl_ = nullptr;
    }
    worker_thread_.reset();
}

bool http_server::start() {
    //start http server
    assert(server_impl_);
    worker_thread_ = std::make_shared<std::thread>(std::bind(&server_impl::run, 
        server_impl_));

    //output http server info
    log_info(http_server_moduler, "http server is running.");
    log_info(http_server_moduler, "http server info:[ip=%s, port=%s, doc_root=%s].", 
        http_srv_ip_.c_str(), http_srv_port_.c_str(), http_root_path_.c_str());

    return true;
}

bool http_server::stop() {
    server_impl_->stop();
    worker_thread_->join();
    log_info(http_server_moduler, "http server is stoped.");
    return true;
}

server_impl::server_impl(const std::string& address, const std::string& port, 
    const std::string& doc_root, const std::string &srv_name, const std::string &auth_pwd) 
    : io_context_(1), 
      signals_(io_context_), 
      acceptor_(io_context_), 
      connection_manager_(), 
      request_handler_(doc_root, srv_name, auth_pwd) {
    do_await_stop();

    //open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    asio::ip::tcp::resolver resolver(io_context_);
    asio::ip::tcp::endpoint endpoint = 
        *resolver.resolve(address, port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    do_accept();
}

void server_impl::run() {
    //the io_context::run() call will block until all asynchronous operations
    //have finished. While the server is running, there is always at least one
    //asynchronous operation outstanding: the asynchronous accept call waiting
    //for new incoming connections.
    io_context_.run();
}

void server_impl::stop() {
    if (!io_context_.stopped()) {
        io_context_.stop();
    }
}

void server_impl::do_accept() {
    acceptor_.async_accept(
        [this](std::error_code ec, asio::ip::tcp::socket socket) {
        //check whether the server was stopped by a signal before this
        //completion handler had a chance to run.
        if (!acceptor_.is_open())
            return;

        if (!ec) {
            connection_manager_.start(std::make_shared<http::server::connection>(
                std::move(socket), connection_manager_, request_handler_));
        }

        do_accept();
    });
}

void server_impl::do_await_stop() {
    signals_.async_wait(
        [this](std::error_code /*ec*/, int /*signo*/) {
        //the server is stopped by cancelling all outstanding asynchronous
        //operations. Once all operations have finished the io_context::run()
        //call will exit.
        acceptor_.close();
        connection_manager_.stop_all();
    });
}

}
}
