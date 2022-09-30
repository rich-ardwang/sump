/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : CedaAgent.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-07
  *Desc        : Ceda客户端的代理类
  *History     : Add by Lei.Wang 2019-05-07
***************************************************************************************************/
#pragma once
#ifndef __CEDA_AGENT_H__
#define __CEDA_AGENT_H__
#include <string>
#include "CedaApi.h"
#include "Config.h"
#include "ISSMQDefine.h"
#include "cJSON.h"
#include "MQServerDef.h"
#include "SSMQServerFrame.h"

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread.hpp>



class semaphore
{
    unsigned int count_;
    boost::mutex mutex_;
    boost::condition_variable condition_;

public:
    explicit semaphore(unsigned int initial) : count_(initial){}

    void signal()
    {
        {
            boost::lock_guard<boost::mutex> lock(mutex_);
            ++count_;
        }
        condition_.notify_one();
    }
 
    void wait()
    {
        boost::unique_lock<boost::mutex> lock(mutex_);
        while (count_ == 0)
        {
             condition_.wait(lock);
        }
        --count_;
    }
};

class CSSMQServerFrame;
class CCedaAgent : public CedaClient
{
public:
    CCedaAgent();
    virtual ~CCedaAgent();

    bool InitCeda(const std::string &usr, const std::string &pwd, std::string &sLoginsid);
    bool InitCeda(const std::string &loginMsg, std::string &sLoginsid);
    bool InitCeda(const PxyInfo &pxyInfo, std::string &sLoginsid);
    bool StopCeda();
    bool isInitCeda();
    std::string getSid();
    void setFrame(CSSMQServerFrame *pFrame);

    void onMessage(const char* topic, const char* content, int len);
    void onEvent(int nevent);

private:
    void ConnectCedaMQ();
    void loginTimeout();
    void stopTimeout();
    std::string parseJson(const char* content, const char *sFiled);

private:
    bool                 m_cedaInit;
    semaphore            m_cedaLoginSig;
    semaphore            m_cedaStopSig;
    bool                 m_bLoginTimeout;
    bool                 m_bStopTimeout;
    std::string          m_strLoginSid;
    CSSMQServerFrame     *m_pFrame;
};


#endif //__CEDA_AGENT_H__ endif