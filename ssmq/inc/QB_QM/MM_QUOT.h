#pragma once
#include <string>
#include <list>
#include <time.h>

namespace MM_QUOT
{
	//������Ϣ�ṹ
	struct QB_Quotation 
	{
		//����ID
		std::string sQuotationID;
		//����
		std::string sDirect;
		//�ʲ�
		std::string sAssetsType;
		//����
		std::string sTerm;
		//��������
		int nTermType;
		//����
		std::string sCount;
		//�۸�
		std::string sPrice;
		//����ʱ��
		time_t tExpirationTime;
		//������ʱ����
		int nCancelType;
		//��ǩ �ϲ�����Ϣ,�����ǩ����#�ָ�
		std::string sTags;
		//��ע
		std::string sMemo;
		//�����û�
		std::string sCreateUsr;

		void Clear()
		{
			sQuotationID = "";
			sDirect = "";
			sAssetsType = "";
			sTerm = "";
			nTermType = 0;
			sCount = "";
			sPrice = "";
			tExpirationTime = 0;
			nCancelType = 0;
			sTags = "";
			sMemo = "";
			sCreateUsr = "";
		}
	};

	typedef std::list<QB_Quotation> LstQBQuotation;

	//��ϵ����Ϣ�ṹ
	struct QB_Contact
	{
		//��ϵ��ID
		std::string sContactID;
		int nContactType;	//nContactType: 0Ϊ����;1Ϊgroup
		time_t SendTime;
	};

	typedef std::list<QB_Contact> LstQBContact;

	//���۹�����Ϣ
	struct QB_QuotationMngInfo
	{
		Quotation Quot;
		LstContact Contacts;
		int nMngType;	//0Ϊ����, 1Ϊ����
		int nOpFlag;	//0Ϊ���� 1Ϊ�����޸� 2Ϊ��ϵ���޸�
	};

	typedef std::list<QB_QuotationMngInfo> LstQBQuotationMngInfo;


	//��ϵ����Ϣ
	struct QB_ContactInfo
	{
		Contact ct;
		//sInfo ���ݲ�ͬ����ϵ�����Ͱ������Ϣ�������
		std::string sInfo;
	};

	typedef std::list<QB_ContactInfo> LstQBContactInfo;

	//���۷��ͼ�¼��Ϣ
	struct QB_Quotation_History
	{
		Quotation Quot;
		time_t SendTime;
		ContactInfo CtInfo;
		int nRecType;		//0Ϊ����, 1Ϊ����
	};

	typedef std::list<QB_Quotation_History> LstQBQuotation_History;
}