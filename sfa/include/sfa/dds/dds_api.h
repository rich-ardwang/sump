/*
 *	dds_api.h
 *  Copyright (c) 2019 Sumscope. All rights reserved.
 *
 *  dds API define
*/
#pragma once
#include <sfa/dds/dds_def.h>
#include <sfa/dds/dds_proxy.h>

namespace sfa {
namespace dds {

/*
* 功  能：初始化dds API模块
* 参  数：
*	config_file 配置文件路径
* 返回值：true成功 false失败
*/
SFA_API bool Init(_sfa_in_ const char* config_file);

/*
* 功  能：设置自动重连、重连次数
* 参  数：
*	is_auto 是否自动重连
*	try_times 重试次数
* 返回值：无
*/
SFA_API void SetAutoReconnect(_sfa_in_ bool is_auto, _sfa_in_ int try_times);

/*
* 功  能：登录dds server
* 参  数：
*	client_login_info 客户端信息
*	listener 网络事件监听
* 返回值：无
*/
SFA_API void Login(_sfa_in_ const ClientLoginInfo* login_info, _sfa_in_ const IEventListener *listener);

/*
* 功  能：获取dds业务接口
* 参  数：无
* 返回值：dds业务接口指针
*/
SFA_API IDdsProxy* CreateProxy(void);

/*
* 功  能：释放dds业务接口指针
* 参  数：
*	proxy CreateProxy函数返回的指针地址
* 返回值：无
*/
SFA_API void ReleaseProxy(_sfa_inout_ IDdsProxy** proxy);

/*
* 功  能：登出dds server
* 参  数：无
* 返回值：无
*/
SFA_API void Logout(void);

/*
* 功  能：释放dds API模块
* 参  数：无
* 返回值：无
*/
SFA_API void Uninit(void);

}
}
