#pragma once
#include <string>
#include <sfa/msg/msg_def.h>

namespace sfa {
namespace msg {

//�ֶ�����
enum class FieldType{
	INTEGER,		 //����
	UNSIGNED_INTEGER,//�޷�������
	DECIMAL,		 //ʮ����
	ASCII,			 //Ascii
	UNIC0DE,		 //Unicode
	BYTE,			 //�ֽ�
	DATE,			 //����
	TIME,			 //ʱ��
	ARRAY,			 //����
	KV_ARRAY		 //Key-value����
};

//�ֶ�����
enum class FieldDesc{
	NORMAL,			 //����
	Null,			 //ռλ��(���ڳ��ģ��)
	PAID,			 //�շ�
	CORRECT,		 //����
	WITHDRAW,		 //����
	RESERVED		 //����
};

/**
 *
 * �����ֶνӿ�
 * ���������ֶ�ʵ�ִ˽ӿ�
 *
*/
class SFA_API IFieldValue{
public:
	virtual FieldType GetFieldType(void) const = 0;
	virtual FieldDesc GetFieldDesc(void) const = 0;

	virtual int64_t ToInt(void) = 0;
	virtual uint64_t ToUInt(void) = 0;
	virtual std::string ToString(void) = 0;
	virtual ByteArray ToBytes(void) = 0;

	virtual IFieldValue* Clone(void) = 0;
};

}
}
