#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include "lava_utils_api.h"
#include "msgbus_callbacks.h"
#include "msgbus_session.h"
#include "msgbus_thread.h"

extern lava_utils_api lava::utils::i_lava_config *get_lava_config();


void Callbacks::setMessageCallback(const MessageCallback &callback) {
    message_callback_ = callback;
}

void Callbacks::messages(const msgbus::Message *msgs, int count) {
    for (int i = 0; i < count; i++)
        message_callback_(msgs[i]);
}

boost::shared_ptr<CallbackOption> concreteCallbackOption(emMsgbusType type, 
    boost::shared_ptr<MsgbusThread> reciever, const std::string &que_name, 
    boost::shared_ptr<Session> session_ptr) {
    boost::shared_ptr<CallbackOption> option(boost::make_shared<CallbackOption>());
    option->queueName_ = que_name;
    session_ptr->set_connection(&reciever->connection());
    option->session_ = session_ptr;
    switch(type) {
    case kDirectMsgbus:
        option->type_ = msgbus::DeliverDirect;
        option->callback_ = boost::bind(&Session::onMessage, option->session_, _1);
        break;
    case kFanoutMsgbus:
        option->type_ = msgbus::DeliverFanout;
        option->callback_ = boost::bind(&Session::onFanoutMessage, option->session_, _1);
    }

    return option;
};

boost::shared_ptr<Callbacks> CallbacksFactory::concrete(const boost::shared_ptr<CallbackOption> option) {
    boost::shared_ptr<Callbacks> callbacks(boost::make_shared<Callbacks>());
    callbacks->name(option->queueName_);
    callbacks->type(option->type_);
    callbacks->setMessageCallback(option->callback_);
    return callbacks;
}
