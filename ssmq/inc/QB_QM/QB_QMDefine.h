#pragma once

#ifdef QB_QM_EXPORTS
#define QB_QM_API __declspec(dllexport)
#else
#define QB_QM_API __declspec(dllimport)
#endif

#include <string>
#include <list>
#include <set>
#include <map>
#include <time.h>

namespace QB_QM
{

	typedef std::set<unsigned long long> SetUInt64;

    typedef std::set<std::string> SetString;

	typedef std::map<std::string, std::string> KeyValues;

	//֧�ֶ����Ե��ַ���
	struct MultilanguageStr
	{
		//��ʾ�ı�
		std::string sDisplay;
		//�ڲ�ö��ֵ
		std::string sEnumVal;

		MultilanguageStr()
			:sDisplay("")
			,sEnumVal("")
		{}
	};

	typedef std::list<MultilanguageStr> LstMultilanguageStr;

	//������Ϣ
	struct TermInfo
	{
		//��ʾ�ı�
		std::string sDisplay;
		//��������
		int nTermType;
		//�ڲ�ö��ֵ
		std::list<std::string> LstEnumVal;
	};

	typedef std::list<TermInfo> LstTermInfo;

	//MM������Ϣ�ṹ
	struct Quotation 
	{
		//����ID
		std::string sQuotationID;
		//����
		MultilanguageStr sDirect;
		//�ʲ�
		MultilanguageStr sAssetsType;
		//����(֧�ֶ��),QM��ʾʱ������޵���ʾ�ı��ö��ŷָ�
		LstTermInfo lstTerms;
		//����
		MultilanguageStr sCount;
		//�۸�
		MultilanguageStr sPrice;
		//����ʱ��
		time_t tExpirationTime;
		//������ʱ����
		int nCancelType;
		//��ǩ
		LstMultilanguageStr lstTags;
		//��ע
		std::string sMemo;

		Quotation()
		{
			Clear();
		}

		void Clear()
		{
			sQuotationID = "";
			sDirect = MultilanguageStr();
			sAssetsType = MultilanguageStr();
			lstTerms.clear();
			sCount = MultilanguageStr();
			sPrice = MultilanguageStr();
			tExpirationTime = 0;
			nCancelType = 0;
			lstTags.clear();
			sMemo = "";
		}

		bool operator < (const Quotation& rIn) const
		{
			return sQuotationID < rIn.sQuotationID;
		}
	};

	typedef std::list<Quotation> LstQuotation;

	//��ϵ����Ϣ�ṹ
	struct Contact
	{
		//��ϵ��ID
		unsigned long long nContactID;
		//nContactType: 0Ϊ����;1Ϊgroup
		int nContactType;

		Contact()
			:nContactID(0)
			,nContactType(0)
		{
		}

		bool operator < (const Contact& rIn) const
		{
			if (nContactType != rIn.nContactType)
			{
				return nContactType < rIn.nContactType;
			}
			return nContactID < rIn.nContactID;
		}

        bool operator == (const Contact& rIn) const
        {
            return nContactType == rIn.nContactType && nContactID == rIn.nContactID;
        }
	};

	typedef std::list<Contact> LstContact;

	//���۹�����Ϣ
	struct QuotationMngInfo
	{
		Quotation Quot;
		LstContact Contacts;
		time_t SendTime;
		int nMngType;		//0Ϊ����, 1Ϊ����

		QuotationMngInfo()
			:SendTime(0)
			,nMngType(0)
		{
		}
	};

	typedef std::list<QuotationMngInfo> LstQuotationMngInfo;

	//���۷�����Ϣ
	struct QuotSendInfo
	{
		Contact SendTo;
		LstQuotation lstQuots;
		time_t SendTime;
		int nMngType;		//0Ϊ����, 1Ϊ����

		QuotSendInfo()
			:SendTime(0)
			,nMngType(0)
		{
		}
	};

	typedef std::list<QuotSendInfo> LstQuotSendInfo;

	//���۽�����Ϣ
	struct QuotRecvInfo
	{
		//��������ܵ���(Ⱥ�����)
		Contact RecvFrom;
		//���������(ֻ���Ǹ���)
		Contact SendFrom;
		LstQuotation lstQuots;
		time_t RecvTime;
		int nMngType;		//0Ϊ����, 1Ϊ����

		QuotRecvInfo()
			:RecvTime(0)
			,nMngType(0)
		{
		}
	};

	typedef std::list<QuotRecvInfo> LstQuotRecvInfo;

	//���ۻظ���Ϣ
	struct QuotReplyInfo
	{
		//�ظ���
		Contact ReplyTo;
		//������Ϣ
		Quotation Quot;
	};

	typedef std::list<QuotReplyInfo> LstQuotReplyInfo;


	//������Ϣ
	struct PersonInfo
	{
		//ID
		unsigned long long nID;
		//��ʾ����
		std::string sDisplayName;
		//�ֻ�
		std::string sMobile;
		//�̻�
		std::string sTel;
		//�������
		std::string sInstitution;
		//����ID
		std::string sInstitutionID;
		//��������
		std::string sInstitutionType;
        //QBAccount
        std::string sQBAccount;

		//ͷ��ͼƬ��������Ϣ(��ַͷ+size)
		char* pAvatar;
		size_t nAvatarSize;

		PersonInfo()
			:nID(0)
			,sDisplayName("")
			,sMobile("")
			,sTel("")
			,sInstitution("")
			,sInstitutionID("")
			,sInstitutionType("")
            ,sQBAccount("")
			,pAvatar(NULL)
			,nAvatarSize(0)
		{
		}

        PersonInfo(const PersonInfo& pi)
        {
            nID = pi.nID;
            sDisplayName = pi.sDisplayName;
            sMobile = pi.sMobile;
            sTel = pi.sTel;
            sInstitution = pi.sInstitution;
            sInstitutionID = pi.sInstitutionID;
            sInstitutionType = pi.sInstitutionType;
            sQBAccount = pi.sQBAccount;
            pAvatar = NULL;
            nAvatarSize = 0;

            SetAvatar(pi.pAvatar, pi.nAvatarSize);
        }

		~PersonInfo()
		{
			if (pAvatar)
			{
				delete[] pAvatar;
				pAvatar = NULL;
				nAvatarSize = 0;
			}
		}

		PersonInfo& operator= (const PersonInfo& pi)
		{
			nID = pi.nID;
			sDisplayName = pi.sDisplayName;
			sMobile = pi.sMobile;
			sTel = pi.sTel;
			sInstitution = pi.sInstitution;
			sInstitutionID = pi.sInstitutionID;
			sInstitutionType = pi.sInstitutionType;
            sQBAccount = pi.sQBAccount;
			SetAvatar(pi.pAvatar, pi.nAvatarSize);
			return (*this) ;
		}

		void SetAvatar(const char* pData, size_t nLength)
		{
			if (pAvatar)
			{
				delete[] pAvatar;
				pAvatar = NULL;
				nAvatarSize = 0;
			}
			if (pData)
			{
				pAvatar = new char[nLength];
				nAvatarSize = nLength;
				memcpy(pAvatar, pData, nLength);
			}
		}
	};

	typedef std::list<PersonInfo> LstPersonInfo;

	//����Ϣ
	struct GroupInfo
	{
		//ID
		unsigned long long nID;
		//��ʾ����
		std::string sDisplayName;
		//ͷ��ͼƬ��������Ϣ(��ַͷ+size)
		char* pAvatar;
		size_t nAvatarSize;

		GroupInfo()
			:nID(0)
			,sDisplayName("")
			,pAvatar(NULL)
			,nAvatarSize(0)
		{
		}

        GroupInfo(const GroupInfo& gi)
        {
            nID = gi.nID;
            sDisplayName = gi.sDisplayName;
            pAvatar = NULL;
            nAvatarSize = 0;

            SetAvatar(gi.pAvatar, gi.nAvatarSize);
        }

		~GroupInfo()
		{
			if (pAvatar)
			{
				delete[] pAvatar;
				pAvatar = NULL;
				nAvatarSize = 0;
			}
		}

		GroupInfo& operator= (const GroupInfo& gi)
		{
			nID = gi.nID;
			sDisplayName = gi.sDisplayName;
			SetAvatar(gi.pAvatar, gi.nAvatarSize);
			return (*this) ;
		}

		void SetAvatar(const char* pData, size_t nLength)
		{
			if (pAvatar)
			{
				delete[] pAvatar;
				pAvatar = NULL;
				nAvatarSize = 0;
			}
			if (pData)
			{
				pAvatar = new char[nLength];
				nAvatarSize = nLength;
				memcpy(pAvatar, pData, nLength);
			}
		}
	};

	typedef std::list<GroupInfo> LstGroupInfo;

	//������Ϣ
	struct NewsInfo
	{
		NewsInfo()
			:pPic(NULL)
			,nLen(0)
		{

		}
		NewsInfo(const NewsInfo &info)
            :pPic(NULL)
            ,nLen(0)
		{
			Copy(info);
		}
		~NewsInfo()
		{
			ClearPic();
		}

		NewsInfo& operator= (const NewsInfo &info)
		{
			if (this == &info)
			{
				return *this;
			}

			Copy(info);
			return *this;
		}

		void SetPic(const char *ptr, size_t len)
		{
			ClearPic();
			if (ptr)
			{
				pPic = new char[len];
				nLen = len;
				memcpy(pPic, ptr, len);
			}
		}
		void ClearPic()
		{
			if (pPic)
			{
				delete []pPic;
				pPic = NULL;
				nLen = 0;
			}
		}

		void Copy(const NewsInfo &info)
		{
			sTitle = info.sTitle;
			sURL = info.sURL;
			SetPic(info.pPic, info.nLen);
		}

		//����
		std::string sTitle;
		//url
		std::string sURL;
		//����ͼ ��������Ϣ(��ַͷ+size)
		char* pPic;
		size_t nLen;
	};

    //������Ϣ
    struct NewsInfoV2
    {
		NewsInfoV2():bIsPdf(false)
        {

        }
        NewsInfoV2(const NewsInfoV2 &info)
        {
            Copy(info);
        }
        ~NewsInfoV2()
        {

        }

        NewsInfoV2& operator= (const NewsInfoV2 &info)
        {
            if (this == &info)
            {
                return *this;
            }

            Copy(info);
            return *this;
        }

        void Copy(const NewsInfoV2 &info)
        {
            sTitle = info.sTitle;
            sURL = info.sURL;
            sPicUUID = info.sPicUUID;
			sImage = info.sImage;
			bIsPdf = info.bIsPdf;
        }

        //����
        std::string sTitle;
        //url
        std::string sURL;
        //����ͼ uuid
        std::string sPicUUID;
		std::string sImage;		// base64
		bool		bIsPdf;
    };

    struct PicInfo
    {
        PicInfo()
            :pPic(NULL)
            ,nLen(0)
        {

        }
        PicInfo(const PicInfo &info)
            :pPic(NULL)
            ,nLen(0)
        {
            Copy(info);
        }
        ~PicInfo()
        {
            ClearPic();
        }

        PicInfo& operator= (const PicInfo &info)
        {
            if (this == &info)
            {
                return *this;
            }

            Copy(info);
            return *this;
        }

        void SetPic(const char *ptr, size_t len)
        {
            ClearPic();
            if (ptr)
            {
                pPic = new char[len];
                nLen = len;
                memcpy(pPic, ptr, len);
            }
        }
        void ClearPic()
        {
            if (pPic)
            {
                delete []pPic;
                pPic = NULL;
                nLen = 0;
            }
        }

        void Copy(const PicInfo &info)
        {
            SetPic(info.pPic, info.nLen);
        }

        //����ͼ ��������Ϣ(��ַͷ+size)
        char* pPic;
        size_t nLen;
    };

	struct BondItem
	{
		std::string			m_sBondKey;							//ծȯ����
		std::string			m_sBondID;							//ծȯ����
		std::string			m_sListedMarket;					//CIB/SSE/SZE ����ͬ���г�
		std::string			m_sCombBondKey;						//�����г���׺��BondKey
		std::string			m_sCombBondID;						//�����г���׺��BondCode
		std::string			m_sFullName;						//ծȯȫ��
		std::string			m_sShortName;						//ծȯ���
		std::string			m_sFullNameEn;						//Ӣ��ȫ��
		std::string			m_sShortNameEn;						//Ӣ�ļ��
		std::string			m_sJianpin;							//��ƴ					���룺------
		std::string			m_sQuanpin;							//ȫƴ					���룺------
		std::string			m_sMarketType;						//�г�����				1����2��
		std::string			m_sBondSubType;						//ծ�������
		std::string			m_sIssuerRating;					//��������������,��������
		std::string			m_sBondRating;						//����ծ������
		std::string			m_sListDate;						//��������
		std::string			m_sInterestStartDate;				//��Ϣ��				���룺"19860414"
		std::string			m_sMaturityDate;					//������				���룺��19860414��
		std::string			m_sPaymentDate;						//�ɿ���
		std::string		    m_sOptionDate;						//��Ȩ����			
		std::string			m_sRateType;						//��������				���룺"Shibor","Depo","��Ϣ"
		std::string			m_sOptionType;						//ѡ��Ȩ����			���룺------
		std::string			m_sMaturityTerm;					//��������
		std::string			m_sTermToMaturity;					//�������ޣ� "X+Y"
		std::string			m_sFRNIndexID;						//�������ʴ���
		double				m_fCouponRateCurrent;				//�б�۸�,Ʊ������
		double				m_fPlannedIssueAmount;				//Ԥ�ڷ�����
		std::string			m_sIssuerCode;						//�����˴���
		std::string			m_sInstitutionType;					//��������			���룺------
		std::string			m_sYieldCurveType;					//��������			���룺------
		std::string			m_sEstcleanprice;					// ��ֵ����
		std::string			m_sEstPrice;						// ��ֵ������

		std::string			m_sAuctionDateStart;				//�б꽨����ʼ��
		std::string			m_sAuctionDateEnd;					//�б꽨����ֹ��
		std::string			m_sAnnounceDate;					//��������
		std::string			m_sIssueStartDate;					//������ʼ��
		std::string			m_sIssueEndDate;					//���н�ֹ��
		std::string			m_sIsUnderwriter;					//�Ƿ�����
		int					m_nIsQuotTopflag;					//�Ƿ��ö�,			����: 1:�ö�
		int					m_nIsWarrant;						//�Ƿ񵣱�
		std::string			m_sMortgate;						//����Ѻ				���룺"Y"
		std::string			m_sCrossmarket;						//���г�				���룺"Y","N"
		std::string			m_sIsMunicipal;						//�Ƿ��Ͷ				���룺"Y"
		std::string			m_sRemainDate;						//ʣ�����ޣ���ʱ����

		int		m_nBondType;						//����QM�����ṩ��ֵ ��������
		//enum EBondType
		//{
		//	Bond_Type_Begin = 0,                      // ��ʼ��
		//	Bond_Type_National = Bond_Type_Begin,     // ��ծ
		//	Bond_Type_Financial,                      // ����ծ
		//	Bond_Type_Municipals,                     // �ط�ծ
		//	Bond_Type_ShortFinancial,                 // ����
		//	Bond_Type_Ticket,                         // ��Ʊ
		//	Bond_Type_Enterprise,                     // ��ҵծ
		//	Bond_Type_Others,                         // ����
		//	Bond_Type_End,                            // ������
		//};
	};

	typedef std::list<BondItem> LstBondItem;

	struct MsgInfo
	{
		Contact MsgFrom;
		std::string sMsgDisplay;
		time_t RecvTime;
	};

	typedef std::list<MsgInfo> LstMsgInfo;

	struct BondQuotInfo
	{
		//ծȯΨһ����
		std::string sCombBondKey;
		//Bid��
		std::string sBid;
		//Bid����
		std::string sVolBid;
		//Ofr��
		std::string sOfr;
		//Ofr����
		std::string sVolOfr;
		//����ʱ��
		time_t QuotTime;
	};

	typedef std::list<BondQuotInfo> LstBondQuotInfo;

	struct BondQuotByCode
	{
		//����ID
		std::string sQuotID;
		//������Ϣ
		BondQuotInfo BQInfo;
		//����ʱ��
		time_t tExpirationTime;
		//������ʱ����
		int nCancelType;
		//��ǩ
		LstMultilanguageStr lstTags;
		//��ע
		std::string sMemo;
	};

	typedef std::list<BondQuotByCode> LstBondQuotByCode;

	struct BondQuotByCodeSendInfo
	{
		Contact SendTo;
		LstBondQuotByCode lstQuots;
		time_t SendTime;
		int nMngType;		//0Ϊ����, 1Ϊ����

		BondQuotByCodeSendInfo()
			:SendTime(0)
			,nMngType(0)
		{
		}
	};

	typedef std::list<BondQuotByCodeSendInfo> LstBondQuotByCodeSendInfo;

	//ծȯ���ۻظ�(������)
	struct BondQuotByCodeReplyInfo
	{
		//�ظ���
		Contact ReplyTo;
		//������Ϣ
		BondQuotByCode Quot;
	};

	struct Condition
	{
		MultilanguageStr sName;
		KeyValues ConditionInfo;
	};

	typedef std::list<Condition> LstCondition;

	struct BondQuotByCondition
	{
		std::string sQuotID;
		//����
		MultilanguageStr sDirect;
		//����
		std::string sCount;
		//����
		LstCondition lstConditions;
		//����ʱ��
		time_t tExpirationTime;
		//������ʱ����
		int nCancelType;
		//��ǩ
		LstMultilanguageStr lstTags;
		//��ע
		std::string sMemo;
	};

	typedef std::list<BondQuotByCondition> LstBondQuotByCondition;

	struct BondQuotByConditionSendInfo
	{
		Contact SendTo;
		LstBondQuotByCondition lstQuots;
		time_t SendTime;
		int nMngType;		//0Ϊ����, 1Ϊ����

		BondQuotByConditionSendInfo()
			:SendTime(0)
			,nMngType(0)
		{
		}
	};

	typedef std::list<BondQuotByConditionSendInfo> LstBondQuotByConditionSendInfo;

    //��ϵ����������,ֻ��ѯ�û�,����ѯȺ
    struct ContactSearch
    {
        //�����Ĺؼ���
        std::string sKey;
        //��󷵻ظ���
        int nLimitUser;
        //��ѯ����:0Ϊ��ѯȫ�г�;1Ϊ��ѯ����
        int nSearchType;

        ContactSearch()
            :sKey("")
            ,nLimitUser(0)
            ,nSearchType(0)
        {}
    };

    //��ϵ����������
    struct SearchUserInfo
    {
        //�û�ID
        unsigned long long nUserID;
        //�û�����
        std::string sUserName;
        //��˾���
        std::string sCompanyName;
    };

    typedef std::list<SearchUserInfo> LstSearchUserInfo;


	//ծȯ���ۻظ�(������)
	struct BondQuotByConditionReplyInfo
	{
		//�ظ���
		Contact ReplyTo;
		//������Ϣ
		BondQuotByCondition Quot;
	};


    //==���۷�����ϢV2 ���QMȺ�������Ż�
    //MM����
    struct QuotSendInfoV2_MM
    {
        LstQuotation lstQuots;
        LstContact lstSendTo; 
        time_t SendTime;
        int nMngType;		//0Ϊ����, 1Ϊ����

        QuotSendInfoV2_MM()
            :SendTime(0)
            ,nMngType(0)
        {
        }
    };

    typedef std::list<QuotSendInfoV2_MM> LstMMQuotSendInfoV2;

    //Bond����ByCode
    struct QuotSendInfoV2_BondByCode
    {
        LstBondQuotByCode lstQuots;
        LstContact lstSendTo;        
        time_t SendTime;
        int nMngType;		//0Ϊ����, 1Ϊ����

        QuotSendInfoV2_BondByCode()
            :SendTime(0)
            ,nMngType(0)
        {
        }
    };

    typedef std::list<QuotSendInfoV2_BondByCode> LstBondQuotByCodeSendInfoV2;

    //====================================

    //һ���깺������Ϣ
    struct PurchaseInfo
    {
        //�깺ID
        std::string sPurchaseID;
        //����޸�ʱ��
        time_t ModifyTime;
        //QM����ʾ��ǰ�벿��Ϣ
        std::string sMsgDisplayP1;
        //QM����ʾ�ĺ�벿��Ϣ
        std::string sMsgDisplayP2;

        PurchaseInfo()
            :sPurchaseID("")
            ,ModifyTime(0)
            ,sMsgDisplayP1("")
            ,sMsgDisplayP2("")
        {}
    };

    //һ���깺����ΨһKey
    struct PurchaseUkey
    {
        //�깺ID
        std::string sPurchaseID;
        //����޸�ʱ��
        time_t ModifyTime;

        PurchaseUkey()
            :sPurchaseID("")
            ,ModifyTime(0)
        {}

        PurchaseUkey(const std::string& sPID, time_t mt)
            :sPurchaseID(sPID)
            ,ModifyTime(mt)
        {}

        bool operator < (const PurchaseUkey& rIn) const
        {
            if (sPurchaseID != rIn.sPurchaseID)
            {
                return sPurchaseID < rIn.sPurchaseID;
            }
            return ModifyTime < rIn.ModifyTime;
        }

        bool operator == (const PurchaseUkey& rIn) const
        {
            return sPurchaseID == rIn.sPurchaseID && ModifyTime == rIn.ModifyTime;
        }
    };

    typedef std::list<PurchaseUkey> LstPurchaseUkey;

    //�깺���۵�״̬:1.�깺��ȷ��; 2.�깺��ȷ��; 3.��Ч���깺(�ѱ��޸�); -1.��ѯʧ��;
    typedef std::map<PurchaseUkey, int> MapPurchaseStatus;

    //����״̬��ѯ�ṹ
    struct QuotStatusReq
    {
        //����ID
        std::string sQuotID;
        //���۷���Ŀ��
        Contact Ct;

        QuotStatusReq()
            :sQuotID("")
        {}
    };

    typedef std::list<QuotStatusReq> LstQuotStatusReq;

    //����״̬
    struct QuotStatus
    {
        //����ID
        std::string sQuotID;
        //���۷���Ŀ��
        Contact Ct;
        //����״̬
        //1.��Ч; 2.�ѳ���; -1.��ѯʧ��;
        int nStatus; 

        QuotStatus()
            :sQuotID("")
            ,nStatus(0)
        {}

        QuotStatus(const std::string& q, int s)
            :sQuotID(q)
            ,nStatus(s)
        {}
    };

    typedef std::list<QuotStatus> LstQuotStatus;

    //��ϵ��״̬
    struct ContactStatus
    {
        //��ϵ��
        Contact Ct;
        //��ϵ��״̬
        //-1.�޷���ȡ״̬; 1.����; 2.����; 3.�뿪; 4.æµ;
        int nStatus;

        ContactStatus()
            :nStatus(0)
        {}
    };

    typedef std::list<ContactStatus> LstContactStatus;

    //�Ǻ���������ϵ��
    struct NoInstituteContact
    {
        std::string sQBAccount;//QBID
        std::string sInstituteName;//��������
        std::string sName;//��ʾ����
        unsigned long long ullQMId;//QM��½�˺�
    };
    typedef std::list<NoInstituteContact> LstNoInstituteContact;

    //QBAccount��QMID�Ķ�Ӧ�ṹ
    struct QBAccount2QMID
    {
        std::string sQBAccount;
        unsigned long long nQMID;

        QBAccount2QMID()
            :sQBAccount("")
            ,nQMID(0)
        {}
    };

    typedef std::list<QBAccount2QMID> LstQBAccount2QMID;

    //begin============QM����������ؽṹ============
    //�ؼ���������
    struct QMRemindKeyItem
    {
        //ID
        std::string sID;
        //�ؼ���
        std::string sKeyWord;
        //����״̬:0Ϊδ����;1Ϊ������
        int nStatus;
        //��ʾ˳��
        int nOrder;
        
        QMRemindKeyItem()
            :sID("")
            ,sKeyWord("")
            ,nStatus(0)
            ,nOrder(0)
        {}

    };

    typedef std::list<QMRemindKeyItem> LstQMRKey;

    //����������
    struct QMRemindFriendItem
    {
        //ID
        std::string sID;
        //��ʾ����
        std::string sDisplayName;
        //����QMID
        unsigned long long nQMID;
        //����״̬:0Ϊδ����;1Ϊ������
        int nStatus;
        //��ʾ˳��
        int nOrder;

        QMRemindFriendItem()
            :sDisplayName("")
            ,nQMID(0)
            ,nStatus(0)
            ,nOrder(0)
        {}
    };

    typedef std::list<QMRemindFriendItem> LstQMRFriend;

#define  REMINDTYPE_QM          0x00000001
#define  REMINDTYPE_POPWND      0x00000002

    //QM��������
    struct QMRemindConfig
    {
        //�ؼ�����������
        LstQMRKey KeyConfig;
        //������������
        LstQMRFriend FriendConfig;
        //���ѷ�ʽ,����ͨ����REMINDTYPE_QM��REMINDTYPE_POPWND��λ�����ж�
        unsigned int nRemindType;

        QMRemindConfig()
            :nRemindType(0)
        {}
    };
    //end============QM����������ؽṹ============


    //begin============ծȯ������ؽṹ============

    struct BondShareItem
    {
        //ծȯΨһ����;
        std::string sCombBondKey;
        //ծȯ����
        std::string sBondCode;
        //ծȯ���
        std::string sBondShortName;
        //��ע��Ϣ
        std::string sMemo;

        BondShareItem()
            :sCombBondKey("")
            ,sBondCode("")
            ,sBondShortName("")
            ,sMemo("")
        {}
    };
    
    typedef std::list<BondShareItem> LstBondShare;

    //end============ծȯ������ؽӿ�============
}