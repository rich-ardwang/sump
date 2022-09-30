#pragma once 
#include <sfa/msg/msg.h>
#include <sfa/msg/msg_context.h>

namespace sfa {
namespace msg {

/*
* ��  �ܣ���IMessageת���ɶ�������
* ��  ����
*	message ��Ҫת����IMessage
*	context IMessage�����Ļ���
*	buffer	��������
*	len		������������
* ����ֵ��true�ɹ� falseʧ��
* ��  ע��buffer��Ҫ����delete��ʽ�ͷ��ڴ�
*/
SFA_API bool EncodeMessage(_sfa_in_ const IMessage* message, _sfa_in_ IMessageContext* context, _sfa_out_ uint8_t*& buffer, _sfa_out_ uint32_t& len);

/*
* ��  �ܣ���IMessageת���ɶ�������
* ��  ����
*	message ��Ҫת����IMessage
*	context IMessage�����Ļ���
*	byte_array	byte����
* ����ֵ��true�ɹ� falseʧ��
*/
SFA_API bool EncodeMessage(_sfa_in_ const IMessage* message, _sfa_in_ IMessageContext* context, _sfa_out_ ByteArray& byte_array);

/*
* ��  �ܣ�����������ת����IMessage
* ��  ����
*	buffer	��Ҫת���Ķ�������
*	len		��Ҫת���Ķ�����������
*	context IMessage�����Ļ���
*	message	ת�����IMessage
* ����ֵ��true�ɹ� falseʧ��
* ��  ע��IMessage������delete��ʽ�ͷ��ڴ�
*/
SFA_API bool DecodeMessage(_sfa_in_ const uint8_t* buffer, _sfa_in_ uint32_t len, _sfa_in_ IMessageContext* context, _sfa_out_ IMessage*& message);

/*
* ��  �ܣ�����������ת����IMessage
* ��  ����
*	byte_array ��Ҫת����byte����
*	context IMessage�����Ļ���
*	message	ת�����IMessage
* ����ֵ��true�ɹ� falseʧ��
* ��  ע��IMessage������delete��ʽ�ͷ��ڴ�
*/
SFA_API bool DecodeMessage(_sfa_in_ const ByteArray& byte_array, _sfa_in_ IMessageContext* context, _sfa_out_ IMessage*& message);

}
}
