/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : Config.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-07
  *Desc        : SSMQServerȫ������
  *History     : Add by Lei.Wang 2019-05-07
***************************************************************************************************/
#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
#include "SSRTL\SSLog.h"
#include "LogMarco.h"


#define GET_GOL_CONF    CConfig::getInstance()->GetGlobalConf()

using namespace std;

/* ceda/ATSMQ�������� */
typedef struct tagGatewayInfo {
    string      sGtwHost;
    int         nGtwPort;
    int         nGtwProtocol;
    int         nGtwIsGZip;
} GtwInfo;

/* ������������� */
typedef struct tagProxyInfo {
    int         nPxyEnable;
    int         nPxyType;
    string      sPxyHost;
    int         nPxyPort;
    string      sPxyUsr;
    string      sPxyPwd;
} PxyInfo;

/* CedaClient������� */
typedef struct tagCedaClientInfo {
    string      sCedaBindKeySpr;
    string      sCedaTopicSpr;
    int         nCedaLoginTimeout;
    int         nCedaStopTimeout;
    int         nCedaProxyLogLel;
    string      sCedaProxyLogFile;
    int         nSubsAckHandleCnt;
} CedaCliInfo;

/* ȫ������ */
typedef struct tagConf {
    string          sBrokerUrl;
    int             nMsgQueueSize;
    CedaCliInfo     stCedaCliInfo;
    GtwInfo         stGtwInfo;
    PxyInfo         stPxyInfo;
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
    string      m_confPath; //�����ļ���·��
    gConf       m_gConf;    //ȫ������
};

ss::logger* GetSrvLogger();

#endif //__CONFIG_H__ endif