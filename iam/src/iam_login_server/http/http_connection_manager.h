/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : http_connection_manager.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-02
*Desc        : maintain http connections.
*History     : Add by Lei.Wang 2019-10-02
***************************************************************************************************/
#ifndef __HTTP_CONNECTION_MANAGER_H__
#define __HTTP_CONNECTION_MANAGER_H__
#include <set>
#include "http_connection.h"


namespace http {
namespace server {

//manages open connections so that they may be cleanly stopped when the server
//needs to shut down.
class connection_manager {
public:
    connection_manager(const connection_manager&) = delete;
    connection_manager& operator=(const connection_manager&) = delete;

    //construct a connection manager.
    connection_manager();

    //add the specified connection to the manager and start it.
    void start(connection_ptr c);

    //stop the specified connection.
    void stop(connection_ptr c);

    //stop all connections.
    void stop_all();

private:
    //the managed connections.
    std::set<connection_ptr> connections_;
};

} //namespace server
} //namespace http

#endif /* __HTTP_CONNECTION_MANAGER_H__ */
