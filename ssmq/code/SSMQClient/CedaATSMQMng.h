/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : CedaATSMQMng.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-04-29
  *Desc        : Ceda/ATSMQ消息总线接口实现
  *History     : Add by Lei.Wang 2019-04-29
***************************************************************************************************/
#pragma once
#ifndef __CEDA_ATS_MQ_MNG__
#define __CEDA_ATS_MQ_MNG__

#include <stdio.h>
#include <map>
#include <objbase.h>
#include <sstream>
#include "ISSMQMng.h"
#include "ISSMQDefine.h"
#include "ICedaATSMQMng.h"
#include "MyCSLock.h"
#include "SSRTL\SSLog.h"
#include "LogMarco.h"
#include "SSMQMng.h"
#include "Config.h"



using namespace CedaATSMQ;

namespace ss
{
    class logger;
}

class CMQCallback;
typedef std::map<sdbus::string, CMQCallback*> mapCedaATSMQCallback;

class CCedaATSMQMng : public ICedaATSMQMng,
                      public SSMQ::ISSMQObs
{
public:
    CCedaATSMQMng(void);
    virtual ~CCedaATSMQMng(void);

public:
    /* Interface of ICedaATSMQMng */
    bool InitCeda(CedaEventObs *pObs);
    void LoginCeda(const sdbus::string &user, const sdbus::string &pwd);
    void LoginCeda(const sdbus::string &validKey);
    void ReleaseCeda();
    void SetCedaMngCallback(ICedaATSMQMngCallback* pCallback);
    void CloseCedaDataBus();
    void ReleaseAllCedaObs();
    bool Send(const sdbus::string &topic, const sdbus::string &request, sdbus::string &response, const sdbus::string &svrID, const int timeout = -1);
    void Post(const sdbus::string &topic, const sdbus::string &content);
    void Subscribe(CedaTopicObs *pObs, const sdbus::string &topics, const sdbus::string &svrID);
    void unSubscribe(CedaTopicObs *pObs);

    /* Interface of ICedaATSMQObs */
    sdbus::string GetBindKey();
    SSMQ::SSMQDeliverType GetDeliverType();
    void OnMsgs(const sdbus::Message& msgs);

    /* ceda internal function */
    sdbus::string getCedaBindKey(const sdbus::string &cedaHeader, const sdbus::string &bindKey); //复用消息线路需要为ceda加上特定消息头
    sdbus::string GetCedaClientName();
    bool IsCedaInit();
    std::string GetUUID();

protected:
    void InitConfig();
    bool RegCeda2Server(CedaEventObs *pObs);
    void UnRegCeda2Server();
    bool WaitRegCedaSucess();
    sdbus::string GetUniqueSendID();

private:
    CRITICAL_SECTION        m_cs;
    ICedaATSMQMngCallback   *m_pMngCallback;
    //ss::logger              *m_pLogger;
    bool                    m_bIsCedaInit;
    HANDLE                  m_hRegCedaSucessEvent;
    HANDLE                  m_hCedaSendRetEvent;
    sdbus::string           m_sendBuf;
    unsigned int            m_nUid;
};

CCedaATSMQMng* GetCedaATSMQMngIns();

#endif    //__CEDA_ATS_MQ_MNG__ endif