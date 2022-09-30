#pragma once
#include "iam_session.h"
#include <sfa/dds/dds_listener.h>

namespace sfa {
namespace dds {

bool IAMSession::Init(void) {
	//��ȡAPI license
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
	/* ֻ��һ��*/
	//����IAM
	//����license��֤

	/* token����������Ҫ����*/
	//����IAM
	//�˺���֤
	//�ͷ�IAM����
}

void IAMSession::Logout(void) {

}

void IAMSession::Uninit(void) {

}

}
}
