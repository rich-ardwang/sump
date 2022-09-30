#pragma once
#include <sfa/dds/dds_listener.h>
#include "client_info.h"

namespace sfa {
namespace dds {

//仅内部使用，不对外暴露
enum Event{
	/*iam network event*/
	event_iam_conn_success,	//连接成功
	event_iam_conn_failed,	//连接失败
	event_iam_conn_disconn,	//连接断开
	event_iam_login_success,//登录成功
	event_iam_login_failed,	//登录失败

	/*dds network event*/
	event_dds_conn_success,	//连接成功
	event_dds_conn_failed,	//连接失败
	event_dds_conn_disconn,	//连接断开 
	event_dds_login_success,//登录成功
	event_dds_login_failed	//登录失败
};

class ISession {
public:
	virtual bool Init(void) = 0;
	virtual void SetAutoReconnect(bool is_auto, int try_times) = 0;
	virtual void Connect(const ClientInfo& client_info,
		const IEventListener* listener) = 0;
	virtual void Login(void) = 0;
	virtual void Logout(void) = 0;
	virtual void Uninit(void) = 0;

protected:
	virtual void NotifyListener(uint16_t event) {
		if (event_listener_ != nullptr) {
			event_listener_->OnEvent(event);
		}
	}

protected:
	IEventListener* event_listener_ = nullptr;
};

}
}
