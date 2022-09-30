#include <boost/make_shared.hpp>
#include "msgbus_thread_factory.h"
#include "msgbus_type.h"
#include "lava_utils_api.h"
#include "common.h"

extern lava_utils_api lava::utils::i_lava_config *get_lava_config();


boost::shared_ptr<MsgbusThread> MsgbusThreadFactory::concrete(emMsgbusType type, 
    const std::string &url, const std::string &que_name, 
    boost::shared_ptr<Session> session) {
    boost::shared_ptr<MsgbusThread> messageReceiver(boost::make_shared<MsgbusThread>());
    bool ret = messageReceiver->connect(url);
    if (!ret) {
        log_error(msgbus_thread_factory_moduler, "msgbus connect failed!");
        messageReceiver.reset();
        return messageReceiver;
    }
    boost::shared_ptr<CallbackOption> option = concreteCallbackOption(type, messageReceiver, que_name, session);
    boost::shared_ptr<Callbacks> callbacks = CallbacksFactory::concrete(option);
    messageReceiver->registerListenQueue(callbacks);
    messageReceiver->registerSession(option->queueName_, option->session_);
    return messageReceiver;
}
