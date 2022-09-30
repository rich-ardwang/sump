#include "singleton.h"
#include "msgbus_server.h"
#include "msgbus_session.h"

//need extern using out of this file
iam::login_server::i_msgbus_server *get_msgbus_server() {
    return singleton_lazy<iam::login_server::msgbus_server, iam::login_server::i_msgbus_server>::instance();
}

void release_msgbus_server() {
    singleton_lazy<iam::login_server::msgbus_server, iam::login_server::i_msgbus_server>::release();
}

extern lava_utils_api lava::utils::i_lava_config *get_lava_config();


namespace iam {
namespace login_server {

bool msgbus_server::start() {
    //get msgbus config info
    char *conf_buf = nullptr;
    int conf_len = 0;
    get_lava_config()->value("msgbus.direct.ip", conf_buf, conf_len);
    std::string drt_ip(conf_buf);
    get_lava_config()->value("msgbus.direct.listen_queue", conf_buf, conf_len);
    std::string drt_lsn_q(conf_buf);
    get_lava_config()->value("msgbus.fanout.ip", conf_buf, conf_len);
    std::string fanout_ip(conf_buf);
    get_lava_config()->value("msgbus.fanout.listen_queue", conf_buf, conf_len);
    std::string fanout_lsn_q(conf_buf);
    if (conf_buf) {
        delete[] conf_buf;
        conf_buf = nullptr;
    }
    assert(!drt_ip.empty());
    assert(!drt_lsn_q.empty());
    assert(!fanout_ip.empty());
    assert(!fanout_lsn_q.empty());

    //start message bus direct worker
    log_info(msgbus_server_moduler, "message bus direct worker prepare to start.");
    boost::shared_ptr<MsgbusSession> bus_ss(boost::make_shared<MsgbusSession>());
    direct_worker_ = MsgbusThreadFactory::concrete(kDirectMsgbus, drt_ip, drt_lsn_q, bus_ss);
    if (!direct_worker_.get()) {
        log_error(msgbus_server_moduler, "message bus direct worker start failed!");
        return false;
    }
    log_info(msgbus_server_moduler, "message bus direct worker starting complete.");

    //start message bus fanout worker
    log_info(msgbus_server_moduler, "message bus fanout worker prepare to start.");
    fanout_worker_ = MsgbusThreadFactory::concrete(kFanoutMsgbus, fanout_ip, fanout_lsn_q, bus_ss);
    if (!fanout_worker_.get()) {
        log_error(msgbus_server_moduler, "message bus fanout worker start failed!");
        return false;
    }
    log_info(msgbus_server_moduler, "message bus fanout worker starting complete.");

    //output msgbus info
    log_info(msgbus_server_moduler, "direct msgbus info:[ip=%s, listen_queue=%s].", 
        drt_ip.c_str(), drt_lsn_q.c_str());
    log_info(msgbus_server_moduler, "fanout msgbus info:[ip=%s, listen_queue=%s].", 
        fanout_ip.c_str(), fanout_lsn_q.c_str());

    return true;
}

std::string msgbus_server::get_server_id() {
    return server_id_;
}

void msgbus_server::send(__lv_in const std::string &que_name, 
    __lv_in const std::shared_ptr<msgbus::Message> msgbus_msg) {
    //check input
    if (que_name.empty())
        return;
    if (!msgbus_msg.get())
        return;
    if (!direct_worker_.get())
        log_warn(msgbus_server_moduler, "message bus direct worker not starting!");

    direct_worker_->sendMessage(que_name, *msgbus_msg);
}

void msgbus_server::fanout(__lv_in const std::string &que_name, 
    __lv_in const std::shared_ptr<msgbus::Message> msgbus_msg) {
    //check input
    if (que_name.empty())
        return;
    if (!msgbus_msg.get())
        return;
    if (!direct_worker_.get())
        log_warn(msgbus_server_moduler, "message bus fanout worker not starting!");

    direct_worker_->fanoutMessage(que_name, *msgbus_msg);
}

}
}
