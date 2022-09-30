#pragma once
#include <map>
#include <sfa/msg/msg_header.h>
#include "fields.h"

namespace sfa {
namespace msg {

/**
*
* ��Ϣͷ���������ֶ����:
*	��Ϣ��ʶ�루���룩1�ֽڣ��̶�Ϊ0x10
*
*	״̬�ֶΣ����룩  1�ֽ�
*		ֹͣλ        [0]	1λ
*		��Ϣ����      [1-2] 2λ��ģ����Ϣ0x03��ȫ����Ϣ0x02��������Ϣ0x01
*		����ģ����  [3]   1λ��0�����ڡ�1����
*		����ģ��汾��[4]   1λ��0�����ڡ�1����
*		���ڸ��Ӳ���  [5]   1λ��0�����ڡ�1����
*
*	ģ���ţ���ѡ��  n�ֽڣ�����ֹͣλ���ͱ���
*	ģ��汾�ţ���ѡ��n�ֽڣ�����ֹͣλ���ͱ���
*	���Ӳ�������ѡ��  n�ֽڣ�ͬ��Ϣ���ֶα���
*
*/
class MessageHeader : public IMessageHeader {
public:
	MessageType GetMessageType(void) const override;
	void SetMessageType(MessageType msg_type) override;

	int GetTemplateId(void) const override;
	void SetTemplateId(int template_id) override;

	int GetVersion(void) const override;
	void SetVersion(int version) override;

	IFieldValue* GetExtraField(int fid) const override;
	void AddExtraField(int fid, const IFieldValue* value) override;
	void DelExtraField(int fid) override;

	std::vector<int> GetAllExtraFieldId(void) const override;
	int GetExtraFieldSize(void) const override;

	std::string ToString(void) override;
	IMessageHeader* Clone(void) override;

private:
	MessageType msg_type_ = UNKNOWN; //��Ϣ����:ȫ����������ģ��
	int template_id_ = 0; //ģ����
	int version_ = 0; //ģ��汾��
	Fields extra_fields_; //���Ӳ���
};

}
}