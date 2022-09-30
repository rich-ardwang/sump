#pragma once
#include <sfa/dds/dds_listener.h>
#include "client_info.h"

namespace sfa {
namespace dds {

//���ڲ�ʹ�ã������Ⱪ¶
enum Event{
	/*iam network event*/
	event_iam_conn_success,	//���ӳɹ�
	event_iam_conn_failed,	//����ʧ��
	event_iam_conn_disconn,	//���ӶϿ�
	event_iam_login_success,//��¼�ɹ�
	event_iam_login_failed,	//��¼ʧ��

	/*dds network event*/
	event_dds_conn_success,	//���ӳɹ�
	event_dds_conn_failed,	//����ʧ��
	event_dds_conn_disconn,	//���ӶϿ� 
	event_dds_login_success,//��¼�ɹ�
	event_dds_login_failed	//��¼ʧ��
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
