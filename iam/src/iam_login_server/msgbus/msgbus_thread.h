/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_thread.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-17
*Desc        : msgbus thread.
*History     : Add by Lei.Wang 2019-09-17
***************************************************************************************************/
#ifndef __MSGBUS_THREAD_H__
#define __MSGBUS_THREAD_H__
#include <string>
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/thread/condition.hpp>
#include "msgbus/connection.h"
#include "msgbus_callbacks.h"

#define msgbus_thread_moduler       "iam.login_server.msgbus_thread"

typedef std::map<std::string, boost::shared_ptr<Callbacks> >    callbacks_hasher;
typedef std::map<std::string, boost::shared_ptr<Session> >      session_haser;


class MsgbusThread : public boost::noncopyable {
public:
    MsgbusThread() {}
    ~MsgbusThread() {}

    bool connect(const std::string &url);
    msgbus::Connection &connection();

    void registerListenQueue(boost::shared_ptr<Callbacks> callback);
    void registerSession(const std::string &name, boost::shared_ptr<Session> session);

    void sendMessage(const std::string &queueName, const msgbus::Message &message);
    void fanoutMessage(const std::string &queueName, const msgbus::Message &message);

private:
    msgbus::Connection          connection_;
    callbacks_hasher            hashCallbacks_;
    session_haser               hashSessions_;
};

#endif /* __MSGBUS_THREAD_H__ */

