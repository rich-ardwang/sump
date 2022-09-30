/*
 *	dds_def.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds API common define
*/
#pragma once
#include <sfa/sfa_macro.h>
#include <sfa/msg/msg.h>
#include <sfa/msg/msg_codec.h>
#include <sfa/msg/value_field.h>
#include <sfa/msg/value_array.h>
#include <sfa/msg/value_array_row.h>
#include <sfa/msg/value_ascii.h>
#include <sfa/msg/value_byte.h>
#include <sfa/msg/value_date.h>
#include <sfa/msg/value_decimal.h>
#include <sfa/msg/value_integer.h>
#include <sfa/msg/value_kv_array.h>
#include <sfa/msg/value_kv_array_row.h>
#include <sfa/msg/value_time.h>
#include <sfa/msg/value_unicode.h>
#include <sfa/msg/value_unsigned_integer.h>
#include <sfa_utils/sfa_base.h>

namespace sfa {
namespace dds {

//代理服务器类型
enum class ProxyType : uint8_t {
	proxy_none = 0,		//不使用代理
	proxy_socks4 = 1,	//socket4代理
	proxy_socks5 = 2,	//socket5代理
	proxy_http = 3		//http代理
};

//代理服务器信息
typedef struct {
	ProxyType type;			//类型
	const char* host;		//主机地址
	const char* port;		//端口
	const char* username;	//用户名
	const char* password;	//密码
}ClientProxyInfo;

//客户端登录信息
typedef struct {
	bool use_proxy;				//是否使用代理
	ClientProxyInfo* proxy_info;//代理信息
	const char* host;			//主机地址
	const char* port;			//端口
	const char* username;		//用户名
	const char* password;		//密码
}ClientLoginInfo;

//历史数据周期
enum class Period {
	one_minute = 1,		//1分钟
	five_minutes = 2,	//5分钟
	thirty_minutes = 3,	//30分钟
	one_hour = 4,		//1小时
	day = 5,			//日
	week = 6,			//周
	month = 7,			//月
	quarter = 8,		//季
	year = 9			//年
};

}
}