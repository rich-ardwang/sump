/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_callbacks.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-17
*Desc        : msgbus callbacks functions.
*History     : Add by Lei.Wang 2019-09-17
***************************************************************************************************/
#ifndef __MSGBUS_CALLBACKS_H__
#define __MSGBUS_CALLBACKS_H__
#include <string>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "lava_utils_api.h"
#include "msgbus/callback.h"
#include "msgbus_base_session.h"
#include "msgbus_type.h"

typedef boost::function<void (const msgbus::Message&)> MessageCallback;


class Callbacks : public msgbus::VectCallback {
public:
    void setMessageCallback(const MessageCallback &callback);
    void messages(const msgbus::Message *msgs, int count);

private:
    MessageCallback     message_callback_;
};

struct CallbackOption {
    std::string                 queueName_;
    msgbus::DeliverType         type_;
    MessageCallback             callback_;
    boost::shared_ptr<Session>  session_;
};

class MsgbusThread;
boost::shared_ptr<CallbackOption> concreteCallbackOption(emMsgbusType type, 
    boost::shared_ptr<MsgbusThread> reciever, const std::string &que_name, 
    boost::shared_ptr<Session> session_ptr);

class CallbacksFactory {
public:
    static boost::shared_ptr<Callbacks> concrete(const boost::shared_ptr<CallbackOption> option);
};

#endif /* __MSGBUS_CALLBACKS_H__ */

