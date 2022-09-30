#include <iostream>
#include "client_demo_impl.h"

//sdn message填数据
sfa::msg::IMessage* FillMessage(sfa::msg::IMessage* msg) {
	assert(msg != nullptr);
	msg->GetHeader()->SetMessageType(sfa::msg::IMessageHeader::MessageType::FULL);
	std::string temp = "CN190210.IBBP=SSCP是是";
	sfa::msg::ByteArray bytes;
	for (size_t i = 0; i < temp.length(); i++) {
		bytes.push_back(temp[i]);
	}
	msg->GetBody()->AddField(1, &sfa::msg::ByteValue(bytes));
	//msg->GetBody()->AddField(2, &sfa::msg::IntegerValue(1564644600000));

/*	std::map<std::string, std::string> data = {
		{"bk", "N0001812019NCD044"},
		{"bklm" , "N0001812019NCD044CIB"},
		{"cid" , "4"},
		{"cn" , "PATR"},
		{"ct" , "1568027752"},
		{"ds" , "2"},
		{"exc" , "1"},
		{"fbar", "1"},
		{"gc" , "111980881"},
		{"gsn" , "19四川天府银行CD044"},
		{"id" , "913744005"},
		{"lm" , "CIB"},
		{"mt" , "1568027752"},
		{"np" , "98.3892"},
		{"pri" , "3.05"},
		{"ps" , "1"},
		{"qt" , "3"},
		{"reb" , "0"},
		{"sts" , "1"},
		{"sym" , "-1"},
		{"vol" , "20000"},
		{"yd" , "3.05"},
		{"ver" , "18850"}
	};
	sfa::msg::ArrayValue array;
	array.AddColType(sfa::msg::FieldType::ASCII);
	array.AddColType(sfa::msg::FieldType::ASCII);

	for (auto iter = data.begin(); iter != data.end(); ++iter) {
		sfa::msg::ArrayRowValue* row = array.AddRow();
		row->AppendValue(&sfa::msg::AsciiValue(iter->first));
		row->AppendValue(&sfa::msg::AsciiValue(iter->second));
	}
	msg->GetBody()->AddField(1, &array);*/

	return msg;
}

//测试sdn message编解码
void TestSdnMessage(void) {
	uint8_t* buf = nullptr;
	uint32_t len = 0;

	//编码
	sfa::msg::IMessageContext* msg_context = sfa::msg::NewMessageContext();
	sfa::msg::IMessage* msg = FillMessage(sfa::msg::NewMessage());
	sfa::msg::EncodeMessage(msg, msg_context, buf, len);
	sfa::msg::DeleteMessage(&msg);

	//解码
	sfa::msg::DecodeMessage(buf, len, msg_context, msg);
	std::string str = msg->ToString();
	sfa::msg::DeleteMessage(&msg);
	sfa::msg::DeleteMessageContext(&msg_context);
}

//测试请求/订阅业务
void TestReqSubscribe(void) {
	ClientDemoImpl client;
	client.StartWork();
	std::cout << "\nPress enter key to stop working..." << std::endl;
	std::cin.get();
	client.StopWork();
}

int main(){
	//TestSdnMessage();
	TestReqSubscribe();
}

