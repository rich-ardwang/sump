/*
 *	i_dds_proxy.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds API common define
*/
#pragma once
#include <sfa/dds/dds_def.h>
#include <sfa/dds/dds_listener.h>

namespace sfa {
namespace dds {

class IDdsProxy {
public:
	/*
	* ��  �ܣ��첽������ʷ���ݽӿ�
	* ��  ����
	*	msg_req		�������(sdnmsg)
	*	listener	��Ϣ����
	* ����ֵ��
	*	����ID 0�ǷǷ�ֵ����Χ��1~0xffffffff
	*/
	virtual uint32_t AsynRequest(_sfa_in_ const msg::IMessage* msg_req, _sfa_in_ const IMsgListener* listener) = 0;

	/*
	* ��  �ܣ�ͬ��������ʷ���ݽӿ�
	* ��  ����
	*	msg_req			�������(sdnmsg)
	*	msg_reply		Ӧ������(sdnmsg)
	*	timeout_seconds ����ʱ����λ���룩��Ĭ��3�볬ʱ
	* ����ֵ��
	*	true�ɹ�  falseʧ��
	*/
	virtual bool Request(_sfa_in_ const msg::IMessage* msg_req, _sfa_out_ msg::IMessage*& msg_reply, _sfa_in_ int32_t timeout_seconds=3) = 0;

	/*
	* ��  �ܣ�����
	* ��  ����
	*	ic				ic�ַ���
	*	filter_fields	�����������ݴ�������field�ֶ�
	*	listener		��Ϣ��������
	* ����ֵ��
	*	true�ɹ�  falseʧ��
	*/
	virtual bool Subscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) = 0;

	/*
	* ��  �ܣ��˶�
	* ��  ����
	*	ic				ic�ַ���
	*	filter_fields	�����������ݴ�������field�ֶ�
	*	listener		��Ϣ��������
	* ����ֵ����
	*/
	virtual void Unsubscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) = 0;
};

}
}

