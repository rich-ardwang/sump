/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : Config.h
  *Author      : Lei.Wang
  *Version     : 1.0
  *Date        : 2019-05-07
  *Desc        : SSMQServer全局配置
  *History     : Add by Lei.Wang 2019-05-07
***************************************************************************************************/
#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
#include "SSRTL\SSLog.h"
#include "LogMarco.h"
#include "BaseStringType.h"
#include "BaseStringFunc.h"



#define GET_GOL_CONF    CConfig::getInstance()->GetGlobalConf()
using namespace std;



/*Ceda ATS MQ Data Def */
enum CedaClientEventType {
    CONNECTION_CONNECTING = 0,              //正在连接
    CONNECTION_CONNECTED = 1,               //连接上
    CONNECTION_CLOSED = 2,                  //连接关闭
    CONNECTION_IO_EXCEPTION = 3,            //连接IO错误
    CONNECTION_RECONNECT = 4,               //重新连接
    CONNECTION_LOGINING = 6,                //正在登录
    CONNECTION_LOGIN_SUCCESS = 7,           //登录成功
    CONNECTION_LOST = 8,                    //连接丢失
    CONNECTION_TIMEOUT = 9,                 //连接超时
    CONNECTION_EXCEPTION = 10,

    ERR_CONNECTION_UNKNOW = 40,             //连接未知错误
    ERR_CONNECTION_NULL_USER_PASSWD = 41,   //密码错误
    ERR_CONNECTION_USER_PASSWD = 42,        //密码错误
    ERR_CONNECTION_CLIENTID_EXIST = 43,     //ClientID exist
    ERR_CONNECTION_AUTHORIZE_CA = 44,       //未授权 CA
    ERR_CONNECTION_CHECK_CA = 45,           //CA
    ERR_CONNECTION_READ_CA = 46,            //CA

    ERR_SUBSCRIB_UNKNOW=60,                 //订阅未知错误
    SUCCESS_SUBSCRIB=61,                    //订阅成功
    ERR_SUBSCRIB_PERIMISSION=62,            //NO 订阅权限
    ERR_SUBSCRIB_LOGIN=63,                  //订阅时未登录

    ERR_PUBLISH_UNKNOW=70,                  //发布未知错误
    ERR_PUBLISH_LOGIN=71,                   //NO Login
    ERR_PUBLISH_PERIMISSION=72              //NO 发布权限
};


//ceda/ATSMQ网关配置
typedef struct tagGatewayInfo {
    string      sGtwHost;
    int         nGtwPort;
    int         nGtwProtocol;
    int         nGtwIsGZip;
} GtwInfo;

//代理服务器配置
typedef struct tagProxyInfo {
    int         nPxyEnable;
    int         nPxyType;
    string      sPxyHost;
    int         nPxyPort;
    string      sPxyUsr;
    string      sPxyPwd;
} PxyInfo;

//全局配置
typedef struct tagConf {
    string      sBrokerUrl;
    int         nMsgQueueSize;
    GtwInfo     stGtwInfo;
    PxyInfo     stPxyInfo;
} gConf;


class CConfig
{
public:
    static CConfig *getInstance() {
        static CConfig _inst;
        return &_inst;
    }

public:
    gConf GetGlobalConf();

private:
    CConfig();
    CConfig(string path);
    CConfig(const CConfig &);
    CConfig &operator=(const CConfig &);
    t_string GetModuleDirPath();
    void loadGlobalConfig();

private:
    string      m_confPath; //配置文件的路径
    gConf       m_gConf;    //全局配置
};

ss::logger* GetSrvLogger();


#endif //__CONFIG_H__ endif