/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : ICedaATSMQMng.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-04-29
  *Desc        : Ceda/ATSMQ消息总线封装接口
  *History     : Add by Lei.Wang 2019-04-29
***************************************************************************************************/
#pragma once
#ifndef __I_CEDA_ATS_MQ_MNG__
#define __I_CEDA_ATS_MQ_MNG__

#include "../sdbus/message.h"
#include "ISSMQObs.h"

#ifdef CEDA_ATSMQCLIENT_EXPORTS
#define CEDA_ATSMQCLIENT_API __declspec(dllexport)
#else
#define CEDA_ATSMQCLIENT_API __declspec(dllimport)
#endif

namespace CedaATSMQ
{
    /* ceda/ATSMQ事件监听器接口，应用端连接Ceda需要继承这个导出类 */
    class CedaEventObs : public SSMQ::ISSMQObs
    {
    protected:
        CEDA_ATSMQCLIENT_API CedaEventObs() {}

    public:
        CEDA_ATSMQCLIENT_API virtual ~CedaEventObs() {}

        //客户端监听事件
        CEDA_ATSMQCLIENT_API virtual void OnEvernt(const int nEventID, const std::string sMsg) {}

    public:
        //以下用于实现内部消息路由
        CEDA_ATSMQCLIENT_API sdbus::string GetBindKey();
        CEDA_ATSMQCLIENT_API SSMQ::SSMQDeliverType GetDeliverType();
        CEDA_ATSMQCLIENT_API void OnMsgs(const sdbus::Message& msgs);
    };

    /* ceda/ATSMQ订阅监听器接口，应用端收取订阅消息需要继承这个导出类 */
    class CedaTopicObs : public SSMQ::ISSMQObs
    {
    protected:
        CEDA_ATSMQCLIENT_API CedaTopicObs();

    public:
        CEDA_ATSMQCLIENT_API virtual ~CedaTopicObs() {}

        //客户端监听topic
        CEDA_ATSMQCLIENT_API virtual void onMessage(const sdbus::string topic, const sdbus::string content, int len) {}

    public:
       //以下用于实现内部消息路由
        CEDA_ATSMQCLIENT_API sdbus::string GetBindKey();
        CEDA_ATSMQCLIENT_API SSMQ::SSMQDeliverType GetDeliverType();
        CEDA_ATSMQCLIENT_API void OnMsgs(const sdbus::Message& msgs);

    private:
        sdbus::string   m_obsBindKey;
    };

    /* 管理回调接口，目前仅提供了关闭客户端功能 */
    class ICedaATSMQMngCallback
    {
    protected:
        ICedaATSMQMngCallback() {}

    public:
        virtual ~ICedaATSMQMngCallback() {}

        //总线可以通过该接口关闭客户端
        virtual void CloseCedaClient() = 0;
    };

    /* Ceda/ATSMQ总线控制API */
    class ICedaATSMQMng
    {
    protected:
        ICedaATSMQMng(){}

    public:
        virtual ~ICedaATSMQMng(){}

        //初始化Ceda组件,一个客户端进程只需初始化一次，Ceda连接信息会以Event形式通知客户端
        virtual bool InitCeda(CedaEventObs *pObs) = 0;
        //使用用户名和密码登录ceda/ATSMQ
        virtual void LoginCeda(const sdbus::string &user, const sdbus::string &pwd) = 0;
        //使用ValidationServer验证登录ceda/ATSMQ
        virtual void LoginCeda(const sdbus::string &LoginMsg)= 0;
        //释放CedaATSMQ
        virtual void ReleaseCeda() = 0;
        //设置管理回调接口
        virtual void SetCedaMngCallback(ICedaATSMQMngCallback *pCallback) = 0;
        //关闭当前客户端下的Ceda通道,供客户端升级调用
        virtual void CloseCedaDataBus() = 0;
        //注销所有消息监听接口
        virtual void ReleaseAllCedaObs() = 0;
        //同步发送消息,对应SSMQDT_Direct的方式,注意timeout单位为秒
        virtual bool Send(const sdbus::string &topic, const sdbus::string &request, sdbus::string &response, const sdbus::string &svrID, const int timeout = -1) = 0;
        //异步发送消息,对应SSMQDT_Direct的方式
        virtual void Post(const sdbus::string &topic, const sdbus::string &content) = 0;
        //批量订阅，使用‘|’分隔topic
        virtual void Subscribe(CedaTopicObs *pObs, const sdbus::string &topics, const sdbus::string &svrID) = 0;
        //批量退订消息，使用‘|’分隔topic
        virtual void unSubscribe(CedaTopicObs *pObs) = 0;
    };
}

//获取ICedaATSMQMng接口,需要SSMQClient.lib sdbusd.lib
CEDA_ATSMQCLIENT_API CedaATSMQ::ICedaATSMQMng* GetCedaATSMQMng();

#endif    //__I_CEDA_ATS_MQ_MNG__ endif