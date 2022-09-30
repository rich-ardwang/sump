#pragma once

#include "QB_QMDefine.h"

//Bond��ص������ӿ�

namespace QB_QM
{
	//�ص��ӿ�,��QMʵ��
	class IQMToQBCallback_Bond
	{
	protected:
		IQMToQBCallback_Bond(){}

	public:
		virtual ~IQMToQBCallback_Bond(){}

		//����ծȯ��Ϣ����
		virtual void RequestBondInfosRet(const LstBondItem& LstRet) = 0;
		//����/����ծȯ����(������)
		virtual bool SendBondQuotByCode(const std::string& sMsgID, const LstBondQuotByCodeSendInfoV2& lstSend) = 0;
		//�ظ�ծȯ����(������)
		virtual bool ReplyBondQuotByCode(const BondQuotByCodeReplyInfo& Reply, const std::string& sWndID) = 0;
        //������ѯBond����(������)��״̬����
        virtual void QueryBondQuotByCodeStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet) = 0;
        //��ָ���û�����ծȯ
        virtual bool ShareBond(const LstContact& Contacts, const LstBondShare& lstBS, const std::string& sWndID) = 0;
	};

	class IQMToQB_Bond
	{
	protected:
		IQMToQB_Bond(){}

	public:
		virtual ~IQMToQB_Bond(){}
		
		//ע��Bond��ػص��ӿ�
		virtual void RegCallback(IQMToQBCallback_Bond* pCallback) = 0;
		//����ծȯ��Ϣ
		virtual bool RequestBondInfos() = 0;
		//��ծȯ����
		virtual bool ShowDlgBondDetail(const std::string& sBondKey) = 0;
		//����/����ծȯ����(������)�������
		virtual bool SendBondQuotByCodeRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed) = 0;
        //������ѯBond����(������)��״̬,�첽�ӿ�,��������ID
        virtual std::string QueryBondQuotByCodeStatus(const LstQuotStatusReq& lstReq) = 0;
	};
}


