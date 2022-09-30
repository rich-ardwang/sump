#pragma once

#include "QB_QMDefine.h"

//MM��ص������ӿ�

namespace QB_QM
{
	//�ص��ӿ�,��QMʵ��
	class IQMToQBCallback_MM
	{
	protected:
		IQMToQBCallback_MM(){}

	public:
		virtual ~IQMToQBCallback_MM(){}

		//����/����MM����
		virtual bool SendQuot(const std::string& sMsgID, const LstMMQuotSendInfoV2& lstSend) = 0;
		//�ظ�MM����
		virtual bool ReplyQuot(const QuotReplyInfo& Reply, const std::string& sWndID) = 0;
        //������ѯMM����״̬����
        virtual void QueryMMQuotStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet) = 0;
	};

	class IQMToQB_MM
	{
	protected:
		IQMToQB_MM(){}

	public:
		virtual ~IQMToQB_MM(){}
		
		//ע��MM��ػص��ӿ�
		virtual void RegCallback(IQMToQBCallback_MM* pCallback) = 0;
		//����/�������۽������
		virtual bool SendQuotRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed) = 0;
        //��MM����,Contacts��ѡ����ϵ��ʱ��Ҫ����QM����ϵ���б�
        virtual void OpenMMQuot(const LstContact& Contacts, const std::string& sWndID = "") = 0;
        //������ѯMM����״̬,�첽�ӿ�,��������ID
        virtual std::string QueryMMQuotStatus(const LstQuotStatusReq& lstReq) = 0;
	};
}


