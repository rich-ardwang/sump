#include "client_demo_impl.h"
#include <iostream>
#include <vector>
#include <sfa/msg/value_unicode.h>

bool EventListener::wait_login(void){
	std::mutex mutex;
	std::unique_lock<std::mutex> lock_guard(mutex);
	return cond_.wait_for(lock_guard, std::chrono::seconds(40), [this]() { return login_success_; });
}

void EventListener::OnEvent(uint16_t event){
	switch (event) {
	case event_conn_success:	//连接成功
		break;
	case event_conn_failed:	//连接失败
		break;
	case event_conn_disconn:	//连接断开 
		break;
	case event_login_success: {//登录成功
		login_success_ = true;
		cond_.notify_one();
		break;
	}
	case event_login_failed:	//登录失败
		break;
	default:
		break;
	}
}

//订阅消息应答
void MsgListener::OnReply(const char* ic, const sfa::msg::IMessage* message){
	if (ic) {
		std::cout << "ic: " << ic << std::endl;
	}
}

//请求消息应答
void MsgListener::OnMessage(uint32_t req_id, const sfa::msg::IMessage* message){
	if (message != nullptr) {
		sfa::msg::IMessage* sndmsg = ((sfa::msg::IMessage*)message)->Clone();
		std::string str = sndmsg->ToString();
		std::cout << str << std::endl;
	}
	else {
		std::cout << "reqid:" << req_id << ",message is null\r\n" << std::endl;
	}
}

void ClientDemoImpl::StartWork(void){
	bool inited = sfa::dds::Init("");
	if (inited) {
		sfa::dds::ClientLoginInfo client;
		memset(&client, 0, sizeof(client));
		client.use_proxy = false;
		client.host = "172.16.97.116";
		client.port = "10020";
		client.username = "idb_trader09";
		client.password = "123456";
		sfa::dds::Login(&client, &event_listener_);

		work_thread_ = new WorkThread(this);
		work_thread_->Start();
	}
}

void ClientDemoImpl::StopWork(void){
	if (work_thread_ != nullptr) {
		work_thread_->Stop();
		delete work_thread_;
		work_thread_ = nullptr;
	}

	sfa::dds::ReleaseProxy(&proxy_);
	sfa::dds::Logout();
	sfa::dds::Uninit();
}

void ClientDemoImpl::OnWork(void){
	if (IsLogined()) {
		ReqTick();
		//ReqKline();
		//SubscribeTest();
	}
}

bool ClientDemoImpl::IsLogined(void){
	return event_listener_.wait_login();
}

uint32_t ClientDemoImpl::ReqTick(void){
	if (proxy_ == nullptr) {
		proxy_ = sfa::dds::CreateProxy();
	}
	assert(proxy_ != nullptr);

	sfa::msg::IMessage* sdnmsg = sfa::msg::NewMessage();
	sdnmsg->GetHeader()->SetMessageType(sfa::msg::IMessageHeader::MessageType::FULL);
	sdnmsg->GetBody()->AddField(1, &sfa::msg::AsciiValue("CN190210.IBBP=CNEX"));	//IC
	sdnmsg->GetBody()->AddField(2, &sfa::msg::IntegerValue(0));						//SubType
	sdnmsg->GetBody()->AddField(3, &sfa::msg::IntegerValue(0));						//Interval
	sdnmsg->GetBody()->AddField(5, &sfa::msg::AsciiValue("20209,20205"));			//Fields
	sdnmsg->GetBody()->AddField(6, &sfa::msg::UnicodeValue("2019-09-16 00:00:00"));	//Start
	sdnmsg->GetBody()->AddField(7, &sfa::msg::UnicodeValue("2019-09-17 15:30:00"));	//End
	sdnmsg->GetBody()->AddField(8, &sfa::msg::IntegerValue(10));					//Count
	sdnmsg->GetBody()->AddField(9, &sfa::msg::IntegerValue(1));						//Page
	uint32_t req_id = proxy_->AsynRequest(sdnmsg, &msg_listener_);
	std::cout << "reqid:" << req_id << ",ReqTick\r\n" << std::endl;
	sfa::msg::DeleteMessage(&sdnmsg);
	return req_id;
}

uint32_t ClientDemoImpl::ReqKline(void){
	if (proxy_ == nullptr) {
		proxy_ = sfa::dds::CreateProxy();
	}
	assert(proxy_ != nullptr);

	sfa::msg::IMessage* sdnmsg = sfa::msg::NewMessage();
	sdnmsg->GetHeader()->SetMessageType(sfa::msg::IMessageHeader::MessageType::FULL);
	sdnmsg->GetBody()->AddField(1, &sfa::msg::AsciiValue("CN190210.IBBP=CNEX"));				//IC
	sdnmsg->GetBody()->AddField(2, &sfa::msg::IntegerValue(0));									//SubType
	sdnmsg->GetBody()->AddField(3, &sfa::msg::IntegerValue((int)sfa::dds::Period::one_minute)); //Interval
	sdnmsg->GetBody()->AddField(4, &sfa::msg::AsciiValue("20209"));								//Fields
	sdnmsg->GetBody()->AddField(6, &sfa::msg::UnicodeValue("2019-09-16 00:00:00"));				//Start
	sdnmsg->GetBody()->AddField(7, &sfa::msg::UnicodeValue("2019-09-17 15:30:00"));				//End
	sdnmsg->GetBody()->AddField(8, &sfa::msg::IntegerValue(10));								//Count
	sdnmsg->GetBody()->AddField(9, &sfa::msg::IntegerValue(1));									//Page
	uint32_t req_id = proxy_->AsynRequest(sdnmsg, &msg_listener_);
	std::cout << "reqid:" << req_id << ",ReqKline\r\n" << std::endl;
	sfa::msg::DeleteMessage(&sdnmsg);
	return req_id;
}

void ClientDemoImpl::SubscribeTest(void) {
	if (proxy_ == nullptr) {
		proxy_ = sfa::dds::CreateProxy();
	}
	assert(proxy_ != nullptr);

	char ic1[20] = "CN190210.IBBP=CNEX";
	std::vector<int> fids1 = {20209, 20205};
	/*for (int i = 0; i < 5; i++) {
		fids1.push_back(i);
	}*/

	/*char ic2[20] = "CN190210.IBBP=CNEX";
	std::vector<int> fids2;
	for (int i = 2; i < 5; i++) {
		fids2.push_back(i);
	}

	char ic3[20] = "CN190301.IBBP=CNEX";
	std::vector<int> fids3;
	for (int i = 8; i < 15; i++) {
		fids3.push_back(i);
	}*/

	//subscribe
	proxy_->Subscribe(ic1, fids1, &msg_listener_);
	//proxy_->Subscribe(ic2, fids2, &msg_listener_);
	//proxy_->Subscribe(ic3, fids3, &msg_listener_);

	//unsub
	//proxy_->Unsubscribe(ic1, fids1, &msg_listener_);
	//proxy_->Unsubscribe(ic2, fids2, &msg_listener_);
	//proxy_->Unsubscribe(ic3, fids3, &msg_listener_);
}
