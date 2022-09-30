/*
 *	i_dds_listener.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds listener define
*/
#pragma once
#include <sfa/dds/dds_def.h>
#include <sfa/msg/msg.h>

namespace sfa {
namespace dds {

//事件监听器
class SFA_API IEventListener {
public:
	enum {
		event_conn_success,	//连接成功
		event_conn_failed,	//连接失败
		event_conn_disconn,	//连接断开 
		event_login_success,//登录成功
		event_login_failed	//登录失败
	};

	virtual void OnEvent(_sfa_in_ uint16_t event) = 0;
};

//消息监听器 
class SFA_API IMsgListener : public sfa::base::i_obj {
public:
	//请求响应消息回调
	virtual void OnMessage(_sfa_in_ uint32_t req_id, _sfa_in_ const msg::IMessage* msg) = 0;

	//订阅消息回调
	virtual void OnReply(_sfa_in_ const char* ic, _sfa_in_ const msg::IMessage* msg) = 0;
};

}
}
