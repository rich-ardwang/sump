/*
 *	i_dds_listener.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds listener define
*/
#pragma once
#include <sfa/dds/dds_def.h>
#include <sfa/msg/msg.h>

namespace sfa {
namespace dds {

//�¼�������
class SFA_API IEventListener {
public:
	enum {
		event_conn_success,	//���ӳɹ�
		event_conn_failed,	//����ʧ��
		event_conn_disconn,	//���ӶϿ� 
		event_login_success,//��¼�ɹ�
		event_login_failed	//��¼ʧ��
	};

	virtual void OnEvent(_sfa_in_ uint16_t event) = 0;
};

//��Ϣ������ 
class SFA_API IMsgListener : public sfa::base::i_obj {
public:
	//������Ӧ��Ϣ�ص�
	virtual void OnMessage(_sfa_in_ uint32_t req_id, _sfa_in_ const msg::IMessage* msg) = 0;

	//������Ϣ�ص�
	virtual void OnReply(_sfa_in_ const char* ic, _sfa_in_ const msg::IMessage* msg) = 0;
};

}
}
