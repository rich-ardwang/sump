#pragma once

#include "QB_QMDefine.h"

//һ���깺��ص������ӿ�

namespace QB_QM
{
	//�ص��ӿ�,��QMʵ��
	class IQMToQBCallback_Purchase
	{
	protected:
		IQMToQBCallback_Purchase(){}

	public:
		virtual ~IQMToQBCallback_Purchase(){}

        //��ָ���û������깺��Ϣ
        virtual bool Purchase(const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID) = 0;
        //��ѯ����ָ����ϵ���Ƿ���ʾ�깺��ť����
        virtual void ContactIsDealerRet(const std::string& sReqID, bool bRet) = 0;
        //��ѯ��ǰ�û���Ȩ�޿����Ľ���ԱQMID����
        virtual void GetAllDealerRet(const std::string& sReqID, const SetUInt64& setID) = 0;
        //������ѯ�깺��״̬����
        virtual void QueryPurchaseStatusRet(const std::string& sReqID, const MapPurchaseStatus& mapPStatus) = 0; 
        //ȷ���깺�����깺��״̬
        virtual void ConfirmPurchaseRet(const std::string& sReqID, int nStatus) = 0;
        //��ѯָ����ϵ�����깺�Ƿ��ܱ༭����
        virtual void IsEditPurchaseValidRet(const std::string& sReqID, bool bRet) = 0;
        //�깺״̬�ı�����
        virtual void PurchaseStatusPush(const MapPurchaseStatus& mapPStatus) = 0;
        //�깺��ť�Ƿ���ʾ����
        virtual void ContactIsDealerPush(const Contact& ct, bool bRet) = 0;
        //�깺�༭��ť�Ƿ���ʾ����
        virtual void IsEditPurchaseValidPush(const Contact& ct, bool bRet) = 0;
	};

	class IQMToQB_Purchase
	{
	protected:
		IQMToQB_Purchase(){}

	public:
		virtual ~IQMToQB_Purchase(){}
		
        //ע���깺��ػص��ӿ�
        virtual void RegCallback(IQMToQBCallback_Purchase* pCallback) = 0;
        //��ѯ����ָ����ϵ���Ƿ���ʾ�깺��ť,�첽�ӿ�,��������ID
        virtual std::string ContactIsDealer(const Contact& ct) = 0;
        //��ѯ��ǰ�û���Ȩ�޿����Ľ���ԱQMID,�첽�ӿ�,��������ID
        virtual std::string GetAllDealer() = 0;
        //���깺�Ի���
        virtual void OpenSubscribeDlg(const Contact& ct, const std::string& sWndID = "") = 0;
        //������ѯ�깺��״̬,�첽�ӿ�,��������ID
        virtual std::string QueryPurchaseStatus(const LstPurchaseUkey& lstQuery) = 0;
        //ȷ���깺,�첽�ӿ�,��������ID
        virtual std::string ConfirmPurchase(const std::string& sPurchaseID, time_t ModifyTime) = 0; 
        //��ѯָ����ϵ�����깺�Ƿ��ܱ༭,�첽�ӿ�,��������ID
        virtual std::string IsEditPurchaseValid(const Contact& ct) = 0;
        //�༭�깺����
        virtual void EditPurchase(const Contact& ct, const std::string& sWndID = "") = 0;
	};
}


