#pragma once
#include <condition_variable>
#include <sfa/dds/dds_def.h>
#include <lava/lava_comm/i_message.h>
#include <lava/lava_box/lava_box_api.h>
#include <lava/lava_box/i_proxy.h>
#include "session.h"
#include "context.h"

namespace sfa {
namespace dds {

class Template {
public:
	Template(ISession* session_dds, Context* context);
	virtual ~Template() = default;

	void Register(void);
	void Unregister(void);
	bool WaitRegisterCompleted(void);

public:
	void OnMessage(lava::comm::i_message* msg);
	void OnReply(lava::comm::i_message* msg, const char* topic);

private:
	Template(const Template&) = delete;
	Template& operator=(const Template&) = delete;

	void OnRegister(lava::comm::i_message* msg);
	lava::comm::i_message* BuildSubscribeMsg(void);

private:
	volatile bool is_registered_ = false;
	std::condition_variable	m_cond;
	msg::IMessageContext* msg_context_ = nullptr;
	lava::base::i_auto_ptr<lava::comm::i_message_listener> cedamsg_listener_ = nullptr;
	lava::base::i_auto_ptr<lava::box::i_proxy> lava_proxy_ = nullptr;
};

}
}
