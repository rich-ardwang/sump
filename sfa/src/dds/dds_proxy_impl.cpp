#pragma once
#include "dds_proxy_impl.h"
#include "sfa_utils/sfa_log.h"
#include "dds_session.h"
#include "dds_utils.h"
#include "request_manager.h"

namespace sfa {
namespace dds {

#define sfa_dds_proxy_impl	"sfa.DdsProxyImpl"

DdsProxyImpl::DdsProxyImpl(ISession* session_dds, Context* context){
	assert(session_dds != nullptr);
	assert(context != nullptr);

	request_mgr_ = new RequestManager(session_dds, context);
	assert(request_mgr_ != nullptr);

	template_ = new Template(session_dds, context);
	assert(template_ != nullptr);
	template_->Register();

	//start subscribe manager
	i_subscribe_manager *sub_mgr_ = DdsUtils::get_subscribe_manager();
	if (sub_mgr_) {
		sub_mgr_->set_session(session_dds, context->GetMsgContext());
	}
	else {
		sfa_log_error(sfa_dds_proxy_impl, "[DdsProxyImpl]: get subscribe manager failed!");
	}
}

DdsProxyImpl::~DdsProxyImpl() {
	if (template_ != nullptr) {
		template_->Unregister();
		template_ = nullptr;
	}
	DdsUtils::DeletePtr(&request_mgr_);
	DdsUtils::DeletePtr(&template_);
}

uint32_t DdsProxyImpl::AsynRequest(const msg::IMessage* msg_req, const IMsgListener* listener) {
	return request_mgr_->AsynRequest(msg_req, listener);
}

bool DdsProxyImpl::Request(const msg::IMessage* msg_req, msg::IMessage*& msg_reply, int32_t timeout_seconds /*= 3*/) {
	return request_mgr_->Request(msg_req, msg_reply, timeout_seconds);
}

bool DdsProxyImpl::Subscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) {
	if (template_==nullptr || !template_->WaitRegisterCompleted()) {
		return false;
	}

	i_subscribe_manager *sub_mgr_ = DdsUtils::get_subscribe_manager();
	if (sub_mgr_) {
		return sub_mgr_->register_listener(ic, filter_fields, const_cast<IMsgListener*>(listener));
	}
	return false;
}

void DdsProxyImpl::Unsubscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) {
	i_subscribe_manager *sub_mgr_ = DdsUtils::get_subscribe_manager();
	if (sub_mgr_) {
		sub_mgr_->unregister_listener(ic, filter_fields, const_cast<IMsgListener*>(listener));
	}
}

}
}