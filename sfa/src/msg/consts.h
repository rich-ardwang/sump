#pragma once
#include <sfa/msg/msg_def.h>

namespace sfa {
namespace msg {

const static char* VERSION = "0.1 Build(190902)";	//�汾��

const static Byte MESSAGE_FLAG = 0x10;	//��Ϣ��ʾ��

const static Byte MESSAGE_TYPE_INCREMENTAL = 0x01; //��Ϣ���ͣ�����Ӧ������Ϣ
const static Byte MESSAGE_TYPE_FULL = 0x02; //��Ϣ���ͣ�����Ӧȫ����Ϣ
const static Byte MESSAGE_TYPE_TEMPLATE = 0x03;	//��Ϣ���ͣ�ģ����Ϣ

const static Byte STOP_YES = 0x01; //ֹͣλ��ֹͣ
const static Byte STOP_NO = 0x00; //ֹͣλ������

const static Byte BYTE_YES = 0x01; //����
const static Byte BYTE_NO = 0x00; //������

const static Byte FIELDTYPE_INT64 = 0x01; //Int64
const static Byte FIELDTYPE_UINT64 = 0x02; //UInt64
const static Byte FIELDTYPE_DECIMAL = 0x03; //ʮ����С��
const static Byte FIELDTYPE_ASCII = 0x04; //ASCII
const static Byte FIELDTYPE_UNICODE = 0x05; //Unicode
const static Byte FIELDTYPE_BYTE = 0x06; //Byte
const static Byte FIELDTYPE_DATE = 0x07; //����
const static Byte FIELDTYPE_TIME = 0x08; //ʱ��
const static Byte FIELDTYPE_ARRAY_BEGIN = 0x09; //���鿪ʼ
const static Byte FIELDTYPE_ARRAY_NEWROW = 0x0A; //����������ʼ
const static Byte FIELDTYPE_ARRAY_END = 0x0B; //�������
const static Byte FIELDTYPE_KVARRAY_BEGIN = 0x0C; //KV���鿪ʼ
const static Byte FIELDTYPE_KVARRAY_NEWROW = 0x0D; //KV����������ʼ
const static Byte FIELDTYPE_KVARRAY_END = 0x0E; //KV�������

const static Byte FIELDVALUE_RESERVED = 0x00; //����
const static Byte FIELDVALUE_NULL = 0x01; //ռλ��(���ڳ��ģ��)
const static Byte FIELDVALUE_NORMAL = 0x02; //����
const static Byte FIELDVALUE_PAID = 0x03; //�շ�
const static Byte FIELDVALUE_CORRECT = 0x04; //����
const static Byte FIELDVALUE_WITHDRAW = 0x05; //����

}
}
