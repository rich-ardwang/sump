#include "request_manager.h"
#include <string>
#include <sfa/dds/dds_def.h>
#include <lava/lava_comm/lava_comm_api.h>
#include "dds_session.h"
#include "lava_listener.h"
#include "const.h"

namespace sfa {
namespace dds {
	
static const int FIELD_SDNMSG = 3; //sdnmsg存放于cedamsg body中的位置

void RequestManager::OnMessage(lava::comm::i_message* msg) {
	lava::comm::i_message* cedamsg = msg;
	if (cedamsg == nullptr) {
		return;
	}

	if (cedamsg->get_func_code() != FUNC_CODE_REQUEST) {
		return;
	}

	//ceda msg转换成sdn msg
	lava::comm::i_message_body* body = cedamsg->get_message_body();
	if (body->has_field(FIELD_SDNMSG) != lava::comm::i_message_body::BYTES) {
		return;
	}
	char* buf = nullptr;
	uint16_t buf_len = body->get_bytes(FIELD_SDNMSG, buf);
	msg::IMessage* sdnmsg = nullptr;
	bool decode_succeed = msg::DecodeMessage((uint8_t*)buf, buf_len, msg_context_, sdnmsg);
	if (!decode_succeed) {
		return;
	}

	//分发sdn message给监听者
	uint32_t req_id = cedamsg->get_msg_id();
	for (auto iter = dds_listeners_.begin(); iter != dds_listeners_.end(); ++iter) {
		(*iter)->OnMessage(req_id, sdnmsg);
	}
	msg::DeleteMessage(&sdnmsg);
}

void RequestManager::OnReply(lava::comm::i_message* msg, const char* topic) {
}

RequestManager::RequestManager(ISession* session_dds, Context* context){
	lava_proxy_ = ((DdsSession*)session_dds)->CreateProxy();
	msg_context_ = context->GetMsgContext();

	i_auto_ptr<i_message_listener> listener_lava(new LavaListener<RequestManager>(this));
	if (lava_proxy_) {
		lava_proxy_->advise(listener_lava.get(), lava_listener_cookie_);
	}
}

RequestManager::~RequestManager() {
	if (lava_proxy_) {
		lava_proxy_->unadvise(lava_listener_cookie_);
	}
}

uint32_t RequestManager::AsynRequest(const msg::IMessage* msg_req, const IMsgListener* listener) {
	assert(msg_req != nullptr);
	assert(listener != nullptr);
	if (msg_req == nullptr || listener == nullptr) {
		return 0;
	}

	using namespace lava::base;
	using namespace lava::comm;
	i_message* cedamsg_req = BuildCedaMsg(msg_req);
	if (cedamsg_req == nullptr) {
		return 0;
	}

	dds_listeners_.insert(const_cast<IMsgListener*>(listener));
	return lava_proxy_->request_asyn(cedamsg_req, lava_listener_cookie_);
}

bool RequestManager::Request(const msg::IMessage* msg_req, msg::IMessage*& msg_reply, int32_t timeout_seconds /*= 3*/) {
	assert(msg_req != nullptr);
	assert(msg_reply == nullptr);
	if (msg_req == nullptr || msg_reply != nullptr) {
		return false;
	}

	using namespace lava::comm;
	i_message* cedamsg_req = BuildCedaMsg(msg_req);
	if (cedamsg_req == nullptr) {
		return false;
	}

	i_message* cedamsg_reply = nullptr;
	bool ret = lava_proxy_->request(cedamsg_req, cedamsg_reply, timeout_seconds);
	if (!ret) {
		return false;
	}

	i_message_body* cedamsg_body = cedamsg_reply->get_message_body();
	if (cedamsg_reply == nullptr ||
		cedamsg_body == nullptr ||
		cedamsg_body->has_field(FIELD_SDNMSG) != lava::comm::i_message_body::BYTES) {
		return false;
	}

	char* buf = nullptr;
	uint16_t buf_len = cedamsg_body->get_bytes(FIELD_SDNMSG, buf);
	return msg::DecodeMessage((uint8_t*)buf, buf_len, msg_context_, msg_reply);
}

lava::comm::i_message* RequestManager::BuildCedaMsg(const msg::IMessage* sdnmsg_req) {
	assert(sdnmsg_req != nullptr);
	if (sdnmsg_req == nullptr) {
		return nullptr;
	}

	uint8_t* buf = nullptr;
	uint32_t buf_len = 0;
	bool encode_succeed = msg::EncodeMessage(sdnmsg_req, msg_context_, buf, buf_len);
	if (!encode_succeed) {
		return nullptr;
	}

	lava::comm::i_message* cedamsg_req = get_message(MSG_TYPE_CODE_REQ);
	if (cedamsg_req != nullptr) {
		cedamsg_req->set_func_code(FUNC_CODE_REQUEST);
		cedamsg_req->get_message_body()->add_bytes(FIELD_SDNMSG, (char*)buf, buf_len);
	}
	return cedamsg_req;
}

}
}