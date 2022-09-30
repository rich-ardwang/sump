#pragma once
#include <sfa/dds/dds_api.h>
#include <sfa/dds/dds_def.h>
#include "thread_base.h"

#pragma comment(lib, "sfa_dds.lib")
#pragma comment(lib, "sfa_msg.lib")

//网络事件监听
class EventListener : public sfa::dds::IEventListener {
public:
	virtual void OnEvent(uint16_t event) override;
	bool wait_login(void);

private:
	std::condition_variable	cond_;
	volatile bool login_success_ = false;
};

//消息监听
class MsgListener : public sfa::dds::IMsgListener {
public:
	virtual void OnReply(const char* ic, const sfa::msg::IMessage* message) override;
	virtual void OnMessage(uint32_t req_id, const sfa::msg::IMessage* message) override;

private:
	i_obj_impl(std::atomic<int32_t>);
};

//客户端demo实现
class ClientDemoImpl : public IThreadCallback {
public:
	void StartWork(void);
	void StopWork(void);

protected:
	virtual void OnWork(void) override;

private:
	bool IsLogined(void);
	uint32_t ReqTick(void);
	uint32_t ReqKline(void);
	void SubscribeTest(void);

private:
	WorkThread* work_thread_ = nullptr;
	sfa::dds::IDdsProxy* proxy_ = nullptr;
	MsgListener msg_listener_;
	EventListener event_listener_;
};
