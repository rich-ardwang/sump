#pragma once
#include "dds_session.h"
#include <atomic>

namespace sfa {
namespace dds {

class LavaEventListener : public lava::comm::i_event_listener {
public:
	explicit LavaEventListener(DdsSession* dds_session)
		: dds_session_(dds_session) {
	}

private:
	virtual void on_event(int32_t event_code, void* event_data) {
		if (dds_session_ != nullptr) {
			dds_session_->OnEvent(event_code);
		}
	}
	virtual void on_exception(int32_t error_code, const char* what) {
	}

private:
	DdsSession* dds_session_;
	i_object_impl(std::atomic<int32_t>)
};

//////////////////////////////////////////////////////////////////////////
bool DdsSession::Init(void) {
	init_environment();
	return true;
}

void DdsSession::SetAutoReconnect(bool is_auto, int try_times) {
	is_auto_reconnect_ = is_auto;
	try_times_ = try_times;
	if (lava_proxy_) {
		lava_proxy_->set_auto_reconnect(is_auto, try_times);
	}
}

void DdsSession::Connect(const ClientInfo& client_info, const IEventListener* listener){
	assert(listener != nullptr);
	event_listener_ = const_cast<IEventListener*>(listener);

	client_login_info* login_info = GetLoginInfo(client_info);
	if (login_info != nullptr) {
		lava_proxy_ = create_proxy("test_rdc_req", login_info, false);
		if (lava_proxy_) {
			lava_listener_ = new(std::nothrow) LavaEventListener(this);
			lava_proxy_->interest_event(lava_listener_.get());
			lava_proxy_->set_auto_reconnect(is_auto_reconnect_, try_times_);
		}

		if (login_info->m_proxy_info != nullptr) {
			delete login_info->m_proxy_info;
		}
		delete login_info;
	}
}

client_login_info* DdsSession::GetLoginInfo(const ClientInfo& client_info) {
	ClientInfo& dds_client_info = const_cast<ClientInfo&>(client_info);
	bool use_proxy = dds_client_info.GetLoginInfo()->use_proxy;
	ProxyInfo* proxy_info = dds_client_info.GetLoginInfo()->proxy_info;

	client_login_info* login_info = new(std::nothrow) client_login_info;
	login_info->m_host = dds_client_info.GetHost();
	login_info->m_port = dds_client_info.GetPort();
	login_info->m_username = dds_client_info.GetUserName();
	login_info->m_password = dds_client_info.GetPassword();
	login_info->m_cluster_info = nullptr;
	login_info->m_use_proxy = use_proxy;
	login_info->m_proxy_info = [](ProxyInfo* proxy)->client_proxy_info * {
		if (proxy == nullptr) {
			return nullptr;
		}

		lava::comm::proxy_type type;
		switch (proxy->type) {
		case ProxyType::proxy_none: type = proxy_none; break;
		case ProxyType::proxy_socks4: type = proxy_socks4; break;
		case ProxyType::proxy_socks5: type = proxy_socks5; break;
		case ProxyType::proxy_http: type = proxy_http; break;
		default: type = proxy_none; break;
		}

		client_proxy_info* info = new(std::nothrow) client_proxy_info;
		info->m_proxy_type = type;
		info->m_proxy_host = proxy->host.c_str();
		info->m_proxy_port = proxy->port.c_str();
		info->m_proxy_username = proxy->username.c_str();
		info->m_proxy_password = proxy->password.c_str();
		return info;
	}(proxy_info);
	return login_info;
}

void DdsSession::Login(void) {
	//TO-DO:·¢ËÍTOKEN
	NotifyListener(event_dds_login_success);
}

void DdsSession::Logout(void) {
	if (lava_proxy_) {
		lava_proxy_->cancel_event(lava_listener_.get());
		lava_proxy_ = nullptr;
	}
}

void DdsSession::Uninit(void) {
	clear_environment();
}

i_auto_ptr<i_proxy> DdsSession::CreateProxy(void) {
	return lava_proxy_;
}

void DdsSession::OnEvent(int32_t event) {
	switch (event) {
	case i_event_listener::event_login_success:
		NotifyListener(event_dds_conn_success);
		break;
	case i_event_listener::exception_conn_failed:
		NotifyListener(event_dds_conn_failed);
		break;
	case i_event_listener::exception_login_failed:
		NotifyListener(event_dds_login_failed);
		break;
	default:
		break;
	}
}

}
}
