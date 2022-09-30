#pragma once
#include "dds_api_impl.h"
#include "dds_proxy_impl.h"
#include "iam_session.h"
#include "dds_session.h"
#include "sfa_utils/config.h"
#include "sfa_utils/singleton.h"
#include "dds_utils.h"
#include "subscribe_manager.h"

namespace sfa {
namespace dds {

DdsApiImpl& DdsApiImpl::GetInstance(void) {
	static DdsApiImpl instance;
	return instance;
}

DdsApiImpl::DdsApiImpl() {
	context_ = new(std::nothrow) Context();
	session_iam_ = new(std::nothrow) IAMSession;
	session_dds_ = new(std::nothrow) DdsSession;
}

DdsApiImpl::~DdsApiImpl() {
	DdsUtils::DeletePtr<ISession>(&session_iam_);
	DdsUtils::DeletePtr<ISession>(&session_dds_);
	DdsUtils::DeletePtr<Context>(&context_);
}

bool DdsApiImpl::Init(const char* config_file) {
	if (config_file == nullptr) {
		return false;
	}

	if (!inited_) {
		//init conf with the path or not, default path is ./setting.ini
		sfa::utils::Config::instance().init();

		bool inited = session_iam_->Init();
		if (!inited) {
			return false;
		}

		inited = session_dds_->Init();
		if (!inited) {
			return false;
		}
		inited_ = true;
	}

	return true;
}

void DdsApiImpl::SetAutoReconnect(bool is_auto, int try_times) {
	is_auto_reconnect_ = is_auto;
	if (try_times <= 0) {
		try_times = 3;
	}
	try_times_ = try_times;
}

void DdsApiImpl::Login(const ClientLoginInfo* login_info, const IEventListener* listener) {
	if (login_info==nullptr
		|| listener==nullptr) {
		return;
	}

	if (!logined_) {
		context_->GetClientInfo()->SetProxyInfo(login_info->proxy_info);
		context_->GetClientInfo()->SetServerInfo(login_info->host, login_info->port);
		context_->GetClientInfo()->SetUserInfo(login_info->username, login_info->password);
		event_listener_ = const_cast<IEventListener*>(listener);
		ConnectIAM();
	}
}

void DdsApiImpl::ConnectIAM(void) {
	session_iam_->SetAutoReconnect(is_auto_reconnect_, try_times_);
	session_iam_->Connect(*context_->GetClientInfo(), this);
}

void DdsApiImpl::OnEvent(uint16_t event) {
	switch (event) {
	case event_iam_conn_success: //连接IAM成功，登录IAM
		LoginIAM();
		break;
	case event_iam_conn_failed: //连接IAM失败
		NotifyListener(event_conn_failed);
		break;
	case event_iam_login_success: //登录IAM成功，连接dds
		LogoutIAM();
		ConnectDDS();
		break;
	case event_iam_login_failed: //登录IAM失败
		NotifyListener(event_login_failed);
		break;

	case event_dds_conn_success: //连接dds成功，登录dds
		LoginDDS();
		break;
	case event_dds_conn_failed: //连接dds失败
		NotifyListener(event_conn_failed);
		break;
	case event_dds_login_success: //登录dds成功
		logined_ = true;
		NotifyListener(event_login_success);
		break;
	case event_dds_login_failed: //登录dds失败
		NotifyListener(event_login_failed);
		break;
	case event_dds_conn_disconn: //dds链接断开
		logined_ = false;
		NotifyListener(event_conn_disconn);
		break;
	default:
		break;
	}
}

void DdsApiImpl::LoginIAM(void) {
	session_iam_->Login();
}

void DdsApiImpl::LogoutIAM(void) {
	session_iam_->Logout();
}

void DdsApiImpl::ConnectDDS(void) {
	session_dds_->SetAutoReconnect(is_auto_reconnect_, try_times_);
	session_dds_->Connect(*context_->GetClientInfo(), this);
}

void DdsApiImpl::LoginDDS(void) {
	session_dds_->Login();
}

void DdsApiImpl::NotifyListener(uint16_t event) {
	if (event_listener_ != nullptr) {
		event_listener_->OnEvent(event);
	}
}

IDdsProxy* DdsApiImpl::CreateProxy(void) {
	if (logined_) {
		return new DdsProxyImpl(session_dds_, context_);
	}
	else {
		return nullptr;
	}
}

void DdsApiImpl::ReleaseProxy(IDdsProxy** proxy) {
	DdsUtils::DeletePtr<IDdsProxy>(proxy);

	//release subscribe manager
	sfa::utils::singleton_lazy<subscribe_manager, i_subscribe_manager>::release();
}

void DdsApiImpl::Logout(void) {
	if (!logined_) {
		return;
	}

	session_iam_->Logout();
	session_dds_->Logout();
	logined_ = false;
}

void DdsApiImpl::Uninit(void) {
	if (!inited_) {
		return;
	}

	session_iam_->Uninit();
	session_dds_->Uninit();
	inited_ = false;
}

}
}