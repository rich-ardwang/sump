/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : Config.h
  *Author      : Lei.Wang
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
#include "BaseStringType.h"
#include "BaseStringFunc.h"



#define GET_GOL_CONF    CConfig::getInstance()->GetGlobalConf()
using namespace std;



/*Ceda ATS MQ Data Def */
enum CedaClientEventType {
    CONNECTION_CONNECTING = 0,              //��������
    CONNECTION_CONNECTED = 1,               //������
    CONNECTION_CLOSED = 2,                  //���ӹر�
    CONNECTION_IO_EXCEPTION = 3,            //����IO����
    CONNECTION_RECONNECT = 4,               //��������
    CONNECTION_LOGINING = 6,                //���ڵ�¼
    CONNECTION_LOGIN_SUCCESS = 7,           //��¼�ɹ�
    CONNECTION_LOST = 8,                    //���Ӷ�ʧ
    CONNECTION_TIMEOUT = 9,                 //���ӳ�ʱ
    CONNECTION_EXCEPTION = 10,

    ERR_CONNECTION_UNKNOW = 40,             //����δ֪����
    ERR_CONNECTION_NULL_USER_PASSWD = 41,   //�������
    ERR_CONNECTION_USER_PASSWD = 42,        //�������
    ERR_CONNECTION_CLIENTID_EXIST = 43,     //ClientID exist
    ERR_CONNECTION_AUTHORIZE_CA = 44,       //δ��Ȩ CA
    ERR_CONNECTION_CHECK_CA = 45,           //CA
    ERR_CONNECTION_READ_CA = 46,            //CA

    ERR_SUBSCRIB_UNKNOW=60,                 //����δ֪����
    SUCCESS_SUBSCRIB=61,                    //���ĳɹ�
    ERR_SUBSCRIB_PERIMISSION=62,            //NO ����Ȩ��
    ERR_SUBSCRIB_LOGIN=63,                  //����ʱδ��¼

    ERR_PUBLISH_UNKNOW=70,                  //����δ֪����
    ERR_PUBLISH_LOGIN=71,                   //NO Login
    ERR_PUBLISH_PERIMISSION=72              //NO ����Ȩ��
};


//ceda/ATSMQ��������
typedef struct tagGatewayInfo {
    string      sGtwHost;
    int         nGtwPort;
    int         nGtwProtocol;
    int         nGtwIsGZip;
} GtwInfo;

//�������������
typedef struct tagProxyInfo {
    int         nPxyEnable;
    int         nPxyType;
    string      sPxyHost;
    int         nPxyPort;
    string      sPxyUsr;
    string      sPxyPwd;
} PxyInfo;

//ȫ������
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
    string      m_confPath; //�����ļ���·��
    gConf       m_gConf;    //ȫ������
};

ss::logger* GetSrvLogger();


#endif //__CONFIG_H__ endif