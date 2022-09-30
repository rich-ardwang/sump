#pragma once
#include <set>
#include <sfa/dds/dds_proxy.h>
#include <lava/lava_box/i_proxy.h>
#include "session.h"
#include "context.h"

namespace sfa {
namespace dds {

class RequestManager {
public:
	RequestManager(ISession* session_dds, Context* context);
	virtual ~RequestManager();

	uint32_t AsynRequest(const msg::IMessage* msg_req, const IMsgListener* listener);
	bool Request(const msg::IMessage* msg_req, msg::IMessage*& msg_reply, int32_t timeout_seconds = 3);

	void OnMessage(lava::comm::i_message* msg);
	void OnReply(lava::comm::i_message* msg, const char* topic);

private:
	lava::comm::i_message* BuildCedaMsg(const msg::IMessage* sdnmsg_req);

private:
	uint32_t lava_listener_cookie_ = 0;
	std::set<IMsgListener*> dds_listeners_;
	msg::IMessageContext* msg_context_ = nullptr;
	lava::base::i_auto_ptr<lava::box::i_proxy> lava_proxy_ = nullptr;
};

}
}
