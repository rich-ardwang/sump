/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : ICedaATSMQMng.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-04-29
  *Desc        : Ceda/ATSMQ��Ϣ���߷�װ�ӿ�
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
    /* ceda/ATSMQ�¼��������ӿڣ�Ӧ�ö�����Ceda��Ҫ�̳���������� */
    class CedaEventObs : public SSMQ::ISSMQObs
    {
    protected:
        CEDA_ATSMQCLIENT_API CedaEventObs() {}

    public:
        CEDA_ATSMQCLIENT_API virtual ~CedaEventObs() {}

        //�ͻ��˼����¼�
        CEDA_ATSMQCLIENT_API virtual void OnEvernt(const int nEventID, const std::string sMsg) {}

    public:
        //��������ʵ���ڲ���Ϣ·��
        CEDA_ATSMQCLIENT_API sdbus::string GetBindKey();
        CEDA_ATSMQCLIENT_API SSMQ::SSMQDeliverType GetDeliverType();
        CEDA_ATSMQCLIENT_API void OnMsgs(const sdbus::Message& msgs);
    };

    /* ceda/ATSMQ���ļ������ӿڣ�Ӧ�ö���ȡ������Ϣ��Ҫ�̳���������� */
    class CedaTopicObs : public SSMQ::ISSMQObs
    {
    protected:
        CEDA_ATSMQCLIENT_API CedaTopicObs();

    public:
        CEDA_ATSMQCLIENT_API virtual ~CedaTopicObs() {}

        //�ͻ��˼���topic
        CEDA_ATSMQCLIENT_API virtual void onMessage(const sdbus::string topic, const sdbus::string content, int len) {}

    public:
       //��������ʵ���ڲ���Ϣ·��
        CEDA_ATSMQCLIENT_API sdbus::string GetBindKey();
        CEDA_ATSMQCLIENT_API SSMQ::SSMQDeliverType GetDeliverType();
        CEDA_ATSMQCLIENT_API void OnMsgs(const sdbus::Message& msgs);

    private:
        sdbus::string   m_obsBindKey;
    };

    /* ����ص��ӿڣ�Ŀǰ���ṩ�˹رտͻ��˹��� */
    class ICedaATSMQMngCallback
    {
    protected:
        ICedaATSMQMngCallback() {}

    public:
        virtual ~ICedaATSMQMngCallback() {}

        //���߿���ͨ���ýӿڹرտͻ���
        virtual void CloseCedaClient() = 0;
    };

    /* Ceda/ATSMQ���߿���API */
    class ICedaATSMQMng
    {
    protected:
        ICedaATSMQMng(){}

    public:
        virtual ~ICedaATSMQMng(){}

        //��ʼ��Ceda���,һ���ͻ��˽���ֻ���ʼ��һ�Σ�Ceda������Ϣ����Event��ʽ֪ͨ�ͻ���
        virtual bool InitCeda(CedaEventObs *pObs) = 0;
        //ʹ���û����������¼ceda/ATSMQ
        virtual void LoginCeda(const sdbus::string &user, const sdbus::string &pwd) = 0;
        //ʹ��ValidationServer��֤��¼ceda/ATSMQ
        virtual void LoginCeda(const sdbus::string &LoginMsg)= 0;
        //�ͷ�CedaATSMQ
        virtual void ReleaseCeda() = 0;
        //���ù���ص��ӿ�
        virtual void SetCedaMngCallback(ICedaATSMQMngCallback *pCallback) = 0;
        //�رյ�ǰ�ͻ����µ�Cedaͨ��,���ͻ�����������
        virtual void CloseCedaDataBus() = 0;
        //ע��������Ϣ�����ӿ�
        virtual void ReleaseAllCedaObs() = 0;
        //ͬ��������Ϣ,��ӦSSMQDT_Direct�ķ�ʽ,ע��timeout��λΪ��
        virtual bool Send(const sdbus::string &topic, const sdbus::string &request, sdbus::string &response, const sdbus::string &svrID, const int timeout = -1) = 0;
        //�첽������Ϣ,��ӦSSMQDT_Direct�ķ�ʽ
        virtual void Post(const sdbus::string &topic, const sdbus::string &content) = 0;
        //�������ģ�ʹ�á�|���ָ�topic
        virtual void Subscribe(CedaTopicObs *pObs, const sdbus::string &topics, const sdbus::string &svrID) = 0;
        //�����˶���Ϣ��ʹ�á�|���ָ�topic
        virtual void unSubscribe(CedaTopicObs *pObs) = 0;
    };
}

//��ȡICedaATSMQMng�ӿ�,��ҪSSMQClient.lib sdbusd.lib
CEDA_ATSMQCLIENT_API CedaATSMQ::ICedaATSMQMng* GetCedaATSMQMng();

#endif    //__I_CEDA_ATS_MQ_MNG__ endif