#pragma once
#include "iam_session.h"
#include <sfa/dds/dds_listener.h>

namespace sfa {
namespace dds {

bool IAMSession::Init(void) {
	//读取API license
	return true;
}

void IAMSession::SetAutoReconnect(bool is_auto, int try_times) {
}

void IAMSession::Connect(const ClientInfo& client_info, const IEventListener* listener){
	event_listener_ = const_cast<IEventListener*>(listener);
	NotifyListener(event_iam_conn_success);
}

void IAMSession::Login(void){
	NotifyListener(event_iam_login_success);
	/* 只干一次*/
	//连接IAM
	//进行license认证

	/* token过期整体需要重来*/
	//连接IAM
	//账号认证
	//释放IAM连接
}

void IAMSession::Logout(void) {

}

void IAMSession::Uninit(void) {

}

}
}
