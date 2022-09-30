#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <sfa/msg/msg.h>
#include <sfa/msg/msg_codec.h>
#include <sfa/msg/value_array.h>
#include <sfa/msg/value_kv_array.h>
#include <sfa/msg/value_byte.h>
#include <sfa/msg/value_unicode.h>
#include <sfa/msg/value_date.h>
#include <sfa/msg/value_time.h>
#include <sfa/msg/value_integer.h>
#include <sfa/msg/value_ascii.h>
#include <sfa/msg/value_decimal.h>

using namespace sfa::msg;

//ȫ����Ϣ
void testFullMessage(void){
	time_t t = time(0);

	IMessage* message = NewMessage();
	IMessageContext *context = NewMessageContext();
	IMessageHeader *header = message->GetHeader();
	header->SetMessageType(IMessageHeader::MessageType::FULL);
	header->SetTemplateId(14);
	header->SetVersion(33);
	header->AddExtraField(10, &DecimalValue(8, 3));

	IMessageBody *body = message->GetBody();
	body->AddField(1, &AsciiValue("abc"));
	body->AddField(2, &IntegerValue(-500));
	body->AddField(3, &ByteValue(ByteArray{ 1, 2 }));
	body->AddField(4, &DecimalValue(FieldDesc::WITHDRAW));
	body->AddField(5, &UnicodeValue("���Ĳ���"));
	body->AddField(6, &UnicodeValue(FieldDesc::Null));
	body->AddField(7, &DateValue(t));
	body->AddField(8, &TimeValue(t));

	ArrayValue array;
	array.AddColType(FieldType::ASCII);
	array.AddColType(FieldType::INTEGER);

	ArrayRowValue* row = array.AddRow();
	row->AppendValue(&AsciiValue("aaa"));
	row->AppendValue(&IntegerValue(-1));
	row = array.AddRow();
	row->AppendValue(&AsciiValue("bbb"));
	row->AppendValue(&IntegerValue(10));
	body->AddField(9, &array);

	KVArrayValue kvArray;
	kvArray.SetColType(6, FieldType::ASCII);
	kvArray.SetColType(8, FieldType::INTEGER);

	KVArrayRowValue* kvrow = kvArray.AddRow();
	kvrow->SetValue(6, &AsciiValue("xxx"));
	kvrow->SetValue(8, &IntegerValue(88));
	kvrow = kvArray.AddRow();
	kvrow->SetValue(6, &AsciiValue("yyy"));
	kvrow->SetValue(8, &IntegerValue(99));
	body->AddField(10, &kvArray);

	std::string src = message->ToString();
	uint8_t* buff = nullptr;
	uint32_t len = 0;
	bool bRet = EncodeMessage(message, context, buff, len);

	IMessage* msg = nullptr;
	bRet = DecodeMessage(buff, len, context, msg);
	std::string newstr = msg->ToString();

 	delete context;
 	delete message;
 	delete msg;
}

//������Ϣ
void testUpdateMessage(void){
	//ģ���
	const int TemplateId = 15;

	//���������õ�������
	IMessageContext* contextServer = NewMessageContext();
	//�ͻ����õ�������
	IMessageContext* contextClient = NewMessageContext();

	//����������ȫ����Ϣ
	IMessage* serverFullMsg = NewMessage();

	IMessageHeader *header1 = serverFullMsg->GetHeader();
	header1->SetMessageType(IMessageHeader::MessageType::FULL);
	header1->SetTemplateId(TemplateId);

	IMessageBody *body1 = serverFullMsg->GetBody();
	body1->AddField(1, &AsciiValue("abc"));
	body1->AddField(10, &DecimalValue(5, 3));
	body1->AddField(4, &UnicodeValue(FieldDesc::Null));

	ArrayValue array1;
	array1.AddColType(FieldType::ASCII);
	array1.AddColType(FieldType::INTEGER);

	ArrayRowValue* row1 = array1.AddRow();
	row1->AppendValue(&AsciiValue("aaa"));
	row1->AppendValue(&IntegerValue(-1));
	body1->AddField(11, &array1);

	contextServer->RegisterTemplate(serverFullMsg);
	std::string server = serverFullMsg->ToString();

	uint8_t* buff = nullptr;
	uint32_t len = 0;
	bool bRet = EncodeMessage(serverFullMsg, contextServer, buff, len);

	//�ͻ����յ�ȫ����Ϣ��ע��ģ��
	IMessage* clientFullMsg = nullptr;
	bRet = DecodeMessage(buff, len, contextClient, clientFullMsg);

	contextClient->RegisterTemplate(clientFullMsg);
	std::string client = clientFullMsg->ToString();

	//������������������Ϣ
	IMessage* serverUpdateMsg = NewMessage();
	IMessageHeader* header2 = serverUpdateMsg->GetHeader();
	header2->SetMessageType(IMessageHeader::MessageType::INCREMENTAL);
	header2->SetTemplateId(TemplateId);
	header2->SetVersion(1); //�汾��+1

	IMessageBody* body2 = serverUpdateMsg->GetBody();
	body2->AddField(4, &AsciiValue("newdat"));//�����ֶ�

	ArrayValue array2;
	array2.AddColType(FieldType::ASCII);
	array2.AddColType(FieldType::INTEGER);

	ArrayRowValue* row2 = array2.AddRow();
	row2->AppendValue(&AsciiValue("bbcc"));
	row2->AppendValue(&IntegerValue(2));
	body2->AddField(11, &array2);

	std::string serverupdate = serverUpdateMsg->ToString();

	contextServer->RegisterTemplate(serverUpdateMsg);
	bRet = EncodeMessage(serverUpdateMsg, contextServer, buff, len);

	//�ͻ����յ�������Ϣ�����
	IMessage* clientUpdateMsg = nullptr;
	bRet = DecodeMessage(buff, len, contextClient, clientUpdateMsg);

	std::string stret = clientUpdateMsg->ToString();

	//�ж�ģ���Ƿ��Ѿ����£��Ѿ����µ���Ҫ����ע��
	bool templateUpdate = contextClient->IsTemplateUpdate(clientUpdateMsg);
	if (templateUpdate){
		contextClient->RegisterTemplate(clientUpdateMsg);
	}
	int xx = 0;
}

int _tmain(int argc, _TCHAR* argv[]){
	testFullMessage();
	//testUpdateMessage();
	return 0;
}


