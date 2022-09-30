#include "template.h"
#include <assert.h>
#include "lava_listener.h"
#include "dds_utils.h"
#include "dds_session.h"
#include "const.h"

namespace sfa {
namespace dds {

static const char* TOPIC = "sdn.cdh.rdc.template";

void Template::OnMessage(lava::comm::i_message* msg) {
	if (msg->get_func_code() == FUNC_CODE_TEMPLATE) {
		OnRegister(msg);
	}
}

void Template::OnReply(lava::comm::i_message* msg, const char* topic) {
	OnRegister(msg);
}

void Template::OnRegister(lava::comm::i_message* msg) {
	char* buf = nullptr;
	lava::comm::i_message_body* body = msg->get_message_body();
	uint16_t buf_len = body->get_bytes(3, buf);

	msg::IMessage* sdnmsg = nullptr;
	bool ret = msg::DecodeMessage((uint8_t *)buf, buf_len, msg_context_, sdnmsg);
	if (!ret || sdnmsg == nullptr) {
		is_registered_ = false;
		m_cond.notify_one();
		return;
	}

	std::vector<int> template_ids = sdnmsg->GetBody()->GetAllFieldId();
	for (int template_id : template_ids) {
		msg::IFieldValue* field_value = sdnmsg->GetBody()->GetField(template_id);
		if (field_value == nullptr) {
			continue;
		}
		msg::ByteArray byte_array = field_value->ToBytes();
		msg::IMessage* template_msg = nullptr;
		bool ret = msg::DecodeMessage(byte_array, msg_context_, template_msg);
		if (ret
			&& template_msg != nullptr
			&& msg_context_->IsTemplateUpdate(template_msg)) {
			msg_context_->RegisterTemplate(template_msg);
			msg::DeleteMessage(&template_msg);
		}
	}

	msg::DeleteMessage(&sdnmsg);
	is_registered_ = true;
	m_cond.notify_one();
}

Template::Template(ISession* session_dds, Context* context) {
	msg_context_ = context->GetMsgContext();
	lava_proxy_ = ((DdsSession*)session_dds)->CreateProxy();
	cedamsg_listener_ = new LavaListener<Template>(this);
}

void Template::Register(void) {
	lava::comm::i_message* cedamsg = BuildSubscribeMsg();
	if (cedamsg != nullptr) {
		lava_proxy_->subscribe_with_filter(const_cast<char*>(TOPIC), cedamsg, cedamsg_listener_.get());
	}
}

void Template::Unregister(void) {
	lava::comm::i_message* cedamsg = BuildSubscribeMsg();
	lava_proxy_->unsubscribe_with_filter(const_cast<char*>(TOPIC), cedamsg, cedamsg_listener_.get());
	is_registered_ = false;
}

lava::comm::i_message* Template::BuildSubscribeMsg(void) {
	uint8_t* buf = nullptr;
	uint32_t buf_len = 0;

	msg::IMessage* sdnmsg = msg::NewMessage();
	sdnmsg->GetHeader()->SetMessageType(msg::IMessageHeader::FULL);
	sdnmsg->GetBody()->AddField(2, &msg::IntegerValue(1)); //1Ϊsnapshot+update
	msg::EncodeMessage(sdnmsg, msg_context_, buf, buf_len);
	msg::DeleteMessage(&sdnmsg);

	if (buf == nullptr || buf_len == 0) {
		return nullptr;
	}

	lava::comm::i_message* cedamsg = get_message(MSG_TYPE_CODE_REQ);
	cedamsg->set_func_code(FUNC_CODE_TEMPLATE);
	cedamsg->get_message_body()->add_bytes(3, (char*)buf, buf_len);
	delete[]buf;

	return cedamsg;
}

bool Template::WaitRegisterCompleted(void) {
	std::mutex dumb_mutex;
	std::unique_lock<std::mutex> lock_guard(dumb_mutex);
	return m_cond.wait_for(lock_guard, std::chrono::seconds(50), [this]() { return is_registered_; });
}

}
}