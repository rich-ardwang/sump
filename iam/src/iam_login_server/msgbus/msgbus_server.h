/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_server.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-23
*Desc        : msgbus server will connect to qpid and handle messages from gateway 
*            : server and other servers.
*History     : Add by Lei.Wang 2019-10-23
***************************************************************************************************/
#ifndef __MSGBUS_SERVER_H__
#define __MSGBUS_SERVER_H__
#include <memory>
#include <boost/make_shared.hpp>
#include "lava_utils_api.h"
#include "msgbus_thread_factory.h"
#include "msgbus_type.h"

#define msgbus_server_moduler       "iam.login_server.msgbus_server"


namespace iam {
namespace login_server {

//interfaces of msgbus server
struct i_msgbus_server {
    virtual bool start() = 0;
    virtual std::string get_server_id() = 0;
    virtual void send(__lv_in const std::string &que_name, 
        __lv_in const std::shared_ptr<msgbus::Message> msgbus_msg) = 0;
    virtual void fanout(__lv_in const std::string &que_name, 
        __lv_in const std::shared_ptr<msgbus::Message> msgbus_msg) = 0;
};

//class msgbus_server definition
class msgbus_server : public i_msgbus_server {
public:
    inline msgbus_server() : direct_worker_(nullptr), 
                             fanout_worker_(nullptr) {
        char *tmp = get_uuid();
        std::string uuid(tmp);
        free_uuid(tmp);
        server_id_ = uuid;
    }
    ~msgbus_server() {}

public:
    virtual bool start() override;
    virtual std::string get_server_id() override;
    virtual void send(__lv_in const std::string &que_name, 
        __lv_in const std::shared_ptr<msgbus::Message> msgbus_msg) override;
    virtual void fanout(__lv_in const std::string &que_name, 
        __lv_in const std::shared_ptr<msgbus::Message> msgbus_msg) override;

private:
    std::string                             server_id_;
    boost::shared_ptr<MsgbusThread>         direct_worker_;
    boost::shared_ptr<MsgbusThread>         fanout_worker_;
};

}
}

#endif /* __MSGBUS_SERVER_H__ */
