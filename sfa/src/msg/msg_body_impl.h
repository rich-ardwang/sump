#pragma once
#include <vector>
#include <map>
#include <sfa/msg/msg_body.h>
#include <sfa/msg/value_array.h>
#include <sfa/msg/value_kv_array.h>
#include "fields.h"

namespace sfa {
namespace msg {

/**
*
* ��Ϣ�壬��Ϣ��֣�ȫ���塢������Ϣ�塣
* ����ȫ����Ϣ����n�����ݶ���ɡ�������Ϣ��1������ͼ(PMAP��n���ֽ�)��n�����ݶ���ɡ�
* ÿ�����ݶ��������ֶ���ɣ�
*	�ֶ����� 1�ֽ�
*		�ֶ�����[0-4] 5λ:
*			[0x01]Int64
*			[0x02]UInt64
*			[0x03]ʮ����С��
*			[0x04]ASCII
*			[0x05]Unicode
*			[0x06]Byte
*			[0x07]����
*			[0x08]ʱ��
*			[0x09]���鿪ʼ
*			[0x0A]����������ʼ
*			[0x0B]�������
*			[0x0C]KV���鿪ʼ
*			[0x0D]KV����������ʼ
*			[0x0E]KV�������
*		�ֶ�����[5-7] 3λ:
*			[0x0]����
*			[0x1]ռλ��(���ڳ��ģ��)
*			[0x2]����
*			[0x3]�շ�
*			[0x4]����
*			[0x5]����
*
*	�ֶ�FID  n�ֽڣ�����ֹͣλ���ͱ���
*	�ֶ����� n�ֽ�
*
*/
class MessageBody : public IMessageBody {
public:
	void AddField(int fid, const IFieldValue* value) override;
	IFieldValue* GetField(int fid) const override;
	void DelField(int fid) override;

	std::vector<int> GetAllFieldId(void) const override;
	int GetFieldSize(void) const override;

	std::string ToString(void) override;
	IMessageBody* Clone(void) override;

private:
	std::string ShowArray(ArrayValue* value);
	std::string ShowKVArray(KVArrayValue* value);
	std::string ShowField(IFieldValue* value);
	std::string GetFieldTypeStr(FieldType type);

private:
	Fields fields_; //�ֶ�
};

}
}