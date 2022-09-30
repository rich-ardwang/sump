//-------------------------------------------------------------------------------------
//--Copyright	:	Sumscope
//--FileName	:	ISSMQMng.h 
//--Author		:	cheng.xu@sumscope.com
//--Date		:	2015.01.23
//--Desc		:	��Ϣ���߷�װ�ӿ�
//--History		:	Created by cheng.xu, 2015.01.23
//--				
//-------------------------------------------------------------------------------------

#ifndef __I_SS_MQMNG__
#define __I_SS_MQMNG__

#include "../sdbus/message.h"

#ifdef SSMQCLIENT_EXPORTS
#define SSMQCLIENT_API __declspec(dllexport)
#else
#define SSMQCLIENT_API __declspec(dllimport)
#endif

namespace SSMQ
{
	class ISSMQObs;

	class ISSMQMngCallback
	{
	protected:
		ISSMQMngCallback(){}

	public:
		virtual ~ISSMQMngCallback(){}

		//���߿���ͨ���ýӿڹرտͻ���
		virtual void CloseClient() = 0;
	};

	class ISSMQMng
	{
	protected:
		ISSMQMng(){}

	public:
		virtual ~ISSMQMng(){}

		//��ʼ����Ϣ����
		virtual bool InitServer() = 0;
		//�ͷ���Ϣ����
		virtual bool ReleaseServer() = 0;
		//���ù���ص��ӿ�
		virtual void SetMngCallback(ISSMQMngCallback* pCallback) = 0;
		//�رյ�ǰ�ͻ����µ�����,���ͻ�����������
		virtual void CloseLocalDataBus() = 0;
		//ע����Ϣ�����ӿ�
		virtual bool RegObs(ISSMQObs* pObs) = 0;
        //ע����Ϣ�����ӿ�
        virtual void ReleaseObs(ISSMQObs* pObs) = 0;
        //ע��������Ϣ�����ӿ�
        virtual void ReleaseAllObs() = 0;
		//ͬ��������Ϣ,��ӦSSMQDT_Direct�ķ�ʽ,ע��timeout��λΪ��
		virtual bool Send(const sdbus::string& bindKey , const sdbus::Message& request , sdbus::Message& response , int timeout = -1) = 0;
		//�첽������Ϣ,��ӦSSMQDT_Direct�ķ�ʽ
		virtual bool Post(const sdbus::string& bindKey , const sdbus::Message& msg) = 0;
		//�첽������Ϣ,��ӦSSMQDT_Publish�ķ�ʽ
		virtual bool Publish(const sdbus::string& bindKey , const sdbus::Message& msg) = 0;
        //����һ����ϢID
        virtual sdbus::string GetUniqueMsgID() = 0;
	};
}

//��ȡISSMQMng�ӿ�,��ҪSSMQClient.lib sdbusd.lib
SSMQCLIENT_API SSMQ::ISSMQMng* GetSSMQMng();

#endif