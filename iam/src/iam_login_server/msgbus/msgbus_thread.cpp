#include "msgbus_thread.h"
#include "lava_utils_api.h"


bool MsgbusThread::connect(const std::string &url) {
    return connection_.connect(url);
}

msgbus::Connection &MsgbusThread::connection() {
    return connection_;
}

void MsgbusThread::registerListenQueue(boost::shared_ptr<Callbacks> callback) {
    callbacks_hasher::iterator itr = hashCallbacks_.find(callback->name());
    if (itr != hashCallbacks_.end())
        return;
    hashCallbacks_.insert(std::make_pair(callback->name(), callback));
    connection_.setHandler(callback.get());
}

void MsgbusThread::registerSession(const std::string &name, boost::shared_ptr<Session> session) {
    session_haser::iterator itr = hashSessions_.find(name);
    if (itr != hashSessions_.end())
        return;
    hashSessions_.insert(std::make_pair(name, session));
}

void MsgbusThread::sendMessage(const std::string &queueName, const msgbus::Message &message) {
    bool ret = connection_.send(queueName, message);
    if (!ret)
        log_warn(msgbus_thread_moduler, "failed to send message.%s, type.%s, msgid.%s", 
            queueName.c_str(), message.getType().c_str(), message.getMessageID().c_str());
}

void MsgbusThread::fanoutMessage(const std::string &queueName, const msgbus::Message &message) {
    bool ret = connection_.fanout(queueName, message);
    if (!ret)
        log_warn(msgbus_thread_moduler, "failed to fanout message.%s, type.%s, msgid.%s", 
            queueName.c_str(), message.getType().c_str(), message.getMessageID().c_str());
}
