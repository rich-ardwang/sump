/*
 *	i_dds_proxy.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds API common define
*/
#pragma once
#include <sfa/dds/dds_def.h>
#include <sfa/dds/dds_listener.h>

namespace sfa {
namespace dds {

class IDdsProxy {
public:
	/*
	* 功  能：异步请求历史数据接口
	* 参  数：
	*	msg_req		请求参数(sdnmsg)
	*	listener	消息监听
	* 返回值：
	*	请求ID 0是非法值，范围：1~0xffffffff
	*/
	virtual uint32_t AsynRequest(_sfa_in_ const msg::IMessage* msg_req, _sfa_in_ const IMsgListener* listener) = 0;

	/*
	* 功  能：同步请求历史数据接口
	* 参  数：
	*	msg_req			请求参数(sdnmsg)
	*	msg_reply		应答数据(sdnmsg)
	*	timeout_seconds 请求超时，单位（秒），默认3秒超时
	* 返回值：
	*	true成功  false失败
	*/
	virtual bool Request(_sfa_in_ const msg::IMessage* msg_req, _sfa_out_ msg::IMessage*& msg_reply, _sfa_in_ int32_t timeout_seconds=3) = 0;

	/*
	* 功  能：订阅
	* 参  数：
	*	ic				ic字符串
	*	filter_fields	定制推送数据传回来的field字段
	*	listener		消息监听对象
	* 返回值：
	*	true成功  false失败
	*/
	virtual bool Subscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) = 0;

	/*
	* 功  能：退订
	* 参  数：
	*	ic				ic字符串
	*	filter_fields	定制推送数据传回来的field字段
	*	listener		消息监听对象
	* 返回值：无
	*/
	virtual void Unsubscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) = 0;
};

}
}

