#pragma once

//JS 新增

#define MSG_TYPE_CTRL_HB		1		//控制消息
#define MSG_TYPE_CTRL_PING		2		//控制消息
#define MSG_TYPE_CTRL_JS_HB		3		//控制消息
#define MSG_TYPE_CTRL_SVRUP		10		//服务启动消息，内部使用

#define MSG_TYPE_CTRL_SUB		17		//订阅消息
#define MSG_TYPE_CTRL_UNSUB		18		//退订消息
#define MSG_TYPE_CTRL_LOGIN		19		//登录消息
#define MSG_TYPE_CTRL_LOGOUT	20		//登出消息


#define MSG_TYPE_CTRL_PROXY		251		//服务节点消息，内部使用


#define MSG_TYPE_DATA_HTTP		96		// HTTP消息
#define MSG_TYPE_DATA_PUB		97		// 广播消息
#define MSG_TYPE_DATA_REQ		98		// 请求消息
#define MSG_TYPE_DATA_REPLY		99		// 回应消息
#define MSG_TYPE_DATA_P2P		100		// 点对点消息
#define MSG_TYPE_DATA_PUB_GROUP	101		// publish group消息
#define MSG_TYPE_CODE_REQ		102		// 新版： 请求消息
#define MSG_TYPE_CODE_REPLY		103		// 新版： 回应消息


#define WEBSOCKET_BIN	0x82


#define CTRL_CODE_LOGIN				3	//登陆
#define CTRL_CODE_LOGIN_RESULT		4	//登陆结果
#define CTRL_CODE_LOGOUT			5	//退出
#define CTRL_CODE_SUB				6	//订阅
#define CTRL_CODE_SUB_RESULT		7	//订阅结果
#define CTRL_CODE_UNSUB				8	//退定
#define CTRL_CODE_UNSUB_RESULT		9	//退定结果
//#define CODE_CTL_HEARTBEAT			10	//心跳
//#define CODE_CTL_MESSAGE_BROWSER	11	//浏览消息
#define CTRL_CODE_PING				12	//客户与服务器互相检查是否在线的请求

#define CTRL_CODE_SUB_BATCH			13  //批量订阅消息
#define CTRL_CODE_UNSUB_BATCH		14  //批量退订消息
//#define CODE_CTL_CLIENT_REGISTER		15  //注册客户端信息到zookeeper

//#define CODE_CTL_CLIENT_USED_SERVER		16  //客户端使用服务；
#define CTRL_CODE_SERVER_NOTIFY		17  //服务器端通知；

#define CTRL_CODE_LOGIN_SID				18  //通过SID登陆

#define CTRL_CODE_LOGIN_VS				19  //客户端切换
#define CTRL_CODE_SUBSCRIBE_WITH_IMAGE 20	//
//JS 新增结束


//消息类型
//#define MESSAGE_TYPE_CTL		1		//控制消息
//#define MESSAGE_TYPE_DATA		2		//数据消息
//#define MESSAGE_TYPE_ACK		3		//确认消息


/*
//控制消息的代码值
#define CODE_CTL_SHAKEHANDS			1	//客户端与服务器端握手，获得初始信息
#define CODE_CTL_SHAKEHANDS_RESULT	2	//服务器端返回给客户端握手结果
#define CODE_CTL_LOGIN				3	//登陆
#define CODE_CTL_LOGIN_RESULT		4	//登陆结果
#define CODE_CTL_LOGOUT				5	//退出
#define CODE_CTL_SUBSCRIBE			6	//订阅
#define CODE_CTL_SUBSCRIBE_RESULT	7	//订阅结果
#define CODE_CTL_UNSUBSCRIBE		8	//退定
#define CODE_CTL_UNSUBSCRIBE_RESULT	9	//退定结果
#define CODE_CTL_HEARTBEAT			10	//心跳
#define CODE_CTL_MESSAGE_BROWSER	11	//浏览消息
#define CODE_CTL_PING				12	//客户与服务器互相检查是否在线的请求


#define CODE_CTL_BATCH_SUBSCRIBE		13  //批量订阅消息
#define CODE_CTL_BATCH_UNSUBSCRIBE		14  //批量退订消息
#define CODE_CTL_CLIENT_REGISTER		15  //注册客户端信息到zookeeper


#define CODE_CTL_CLIENT_USED_SERVER		16  //客户端使用服务；
#define CODE_CTL_SERVER_NOTIFY		17  //服务器端通知；

#define CODE_CTL_SID_LOGIN	18  //通过SID登陆

#define CODE_CTL_CLIENT_SWITCH	19  //客户端切换

#define CODE_CTL_CHECK_PING				30	//客户与服务器互相检查是否在线的请求
#define CODE_CTL_JS_HEARTBEAT			31	//JS返回心跳
*/

/*
//CA
#define CTL_CODE_SERVER_CERT		13	//客户请求服务器CA cert
#define CTL_CODE_CA_CLIENT			14	//CA认证
#define CTL_CODE_CA_RESULT			15	//CA认证结果
*/

//#define CODE_CTL_PING_RESULT		13	//查询请求的结果
//#define CODE_CTL_CHANGE_PASSWORD	14	//修改密码
//#define CODE_CTL_CHANGE_PASSWORD_RESULT	15	//修改密码的结果

//确认消息的代码值
#define CODE_ACK_CLIENT_CONFIRMED	1	//消息被客户端确认
#define CODE_ACK_SERVER_CONFIRMED	2	//消息被服务器端确认
#define CODE_ACK_NO_PERMISSION		3	//没有权限

#define CODE_REMOVE_NODE			100	//移除节点信息
#define CODE_CHECK_NODE				101	//检查节点信息
#define CODE_DEL_PROXY				102	//删除代理连接
//#define CODE_SERVICE_UP				110	//服务激活指令
#define TOPIC_ATS_LOGIN			"SYS.ATS.LOGIN"
#define TOPIC_API_LOGIN			"API.ATS.LOGIN"
#define TOPIC_API_REQUESTIMAGE	"API.REQUESTIMAGE"
#define TOPIC_ACS_REQUESTIMAGE	"ACS.REQUESTIMAGE"  //客户端请求镜像消息
#define TOPIC_API_STREAM_SUB	"API.STSUB"			//stream消息订阅
#define TOPIC_API_STREAM_REQ	"API.STREQ"			//stream消息请求

#define MAX_FILENAME_LENGTH		512		//文件名称的最大长度

#define MAX_DESTINATION_LENGTH	255		//Destination的最长长度
#define MAX_MESSAGE_HEAD_LENGTH	535		//每个消息头的最大长度
#define MAX_MESSAGE_BODY_LENGTH	65000	//每个消息体的最大长度
#define MAX_MESSAGE_LENGTH		65535	//每个消息的最大长度
#define MAX_MESSAGE_2X_LENGTH	131070	//每个消息的最大长度的2倍

#define MAX_GROUP_FIELD_LENGTH	2048	//32768

#define MAX_RECORD_FIELD		4095	//消息里面域的最大数

#define MAX_USERNAME_LEN		128		//用户名的长度
#define MAX_PASSWORD_LEN		128		//密码的长度
#define MAX_CLIENT_ID_LEN		128		//Client ID的长度

#define MAX_PERMISSION_GROUP	128		//权限组名称的长度
#define PERMISSION_ALLOW		'A'		//权限容许
#define PERMISSION_DENY			'D'		//权限禁止

#define DMQ_DESTINATION		"$SYS.DMQ"	//Dead Message Queue的地址名称

#define TCP_CLIENT	0
#define WS_CLIENT	1
