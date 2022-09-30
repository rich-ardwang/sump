#pragma once
#include "QB_QMDefine.h"

namespace QB_QM
{
	class IQBToQMCallback
	{
	protected:
		IQBToQMCallback(){}

	public:
		virtual ~IQBToQMCallback(){}

		//�˳�QB,�ر�����ʱ����øýӿ�
		virtual void QuitQB() = 0;
		//ѡ����ϵ�˷���
		virtual bool SelContactsRet(const std::string& sSelID, const LstContact& Contacts) = 0;
		//��ѯ��ϵ����Ϣ����,��ѯʱ��Ч��ID��lstFailed����
		virtual bool QueryContactInfoRet(LstPersonInfo& PersonInfos, LstGroupInfo& GrpInfos, const LstContact& lstFailed) = 0;
		//�༭��ϵ�˷���
		virtual bool EditContactsRet(const std::string& sEditID, const LstContact& Contacts) = 0;
		//����/�������۽������
		virtual bool SendQuotRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed) = 0;
		//ͨ��QM״̬ 1Ϊ����;-1Ϊ����;2Ϊ��½�� 
		virtual void QMStatus(int nStatus) = 0;
		//����ծȯ��Ϣ
		virtual bool RequestBondInfos() = 0;
		//��ծȯ����
		virtual bool ShowDlgBondDetail(const std::string& sBondKey) = 0;
		//��ѯ��ǰ�û���QMID����
		virtual void QueryCurUsrIDRet(unsigned long long UsrID) = 0;
		//��ѯ��ǰ�û�����ID����
		virtual void QueryCurUsrFriendIDsRet(const SetUInt64& setIDs) = 0;
		//��ѯ��ǰ�û�ȺID����
		virtual void QueryCurUsrGroupIDsRet(const SetUInt64& setIDs) = 0;
		//��ǰ�û�Ⱥ�仯ʱ֪ͨQB    JoinGrpIDs:�¼����ȺID    LeaveGrpIDs:�뿪��ȺID
		virtual void CurUsrGroupChange(const SetUInt64& JoinGrpIDs, const SetUInt64& LeaveGrpIDs) = 0;
        //��ǰ�û����ѱ仯ʱ֪ͨQB    AddFriendIDs:�¼Ӻ���ID    DelFriendIDs:ɾ������ID
        virtual void CurUsrFriendChange(const SetUInt64& AddFriendIDs, const SetUInt64& DelFriendIDs) = 0;
        //��ѯQB����λ��
        virtual bool QueryQBWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight) = 0;
        //��ѯQB���ھ��,��sWndIDΪ����Ĭ��Ϊ�����ھ��
        virtual bool QueryQBWndHandle(const std::string& sWndID, HWND& hWnd) = 0;
        //��ѯQB���Ȩ��:MM����--mmquot;
        virtual bool QueryQBPerm(SetString& setPerm) = 0;
        //��MM����,Contacts��ѡ����ϵ��ʱ��Ҫ����QM����ϵ���б�
        virtual void OpenMMQuot(const LstContact& Contacts, const std::string& sWndID) = 0;
        //����QMȫ�г���ϵ�˷���
        virtual void SearchQMContactRet(const std::string& sMsgID, const LstSearchUserInfo& Result) = 0;
        //��ѯ����ָ����ϵ���Ƿ���ʾ�깺��ť
        virtual bool ContactIsDealer(const Contact& ct) = 0;
        //���깺�Ի���
        virtual void OpenSubscribeDlg(const Contact& ct, const std::string& sWndID) = 0;
        //��ѯ��ǰ�û���Ȩ�޿����Ľ���ԱQMID
        virtual void GetAllDealer(const std::string& sReqID) = 0;
		//����/����ծȯ����(������)�������
		virtual bool SendBondQuotByCodeRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed) = 0;
        //QM������Ϣ֪ͨ
        virtual void QMErrInfo(int nErrCode, const std::string& sErrInfo) = 0;
        //������ѯ�깺��״̬
        virtual void QueryPurchaseStatus(const LstPurchaseUkey& lstQuery, MapPurchaseStatus& mapRet) = 0;
        //ȷ���깺, �����깺״̬
        virtual void ConfirmPurchase(const std::string& sReqID, const std::string& sPurchaseID, time_t ModifyTime) = 0; 
        //��ѯָ����ϵ�����깺�Ƿ��ܱ༭
        virtual bool IsEditPurchaseValid(const Contact& ct) = 0;
        //�༭�깺����
        virtual void EditPurchase(const Contact& ct, const std::string& sWndID) = 0;
        //������ѯMM����״̬
        virtual void QueryMMQuotStatus(const std::string& sReqID, const LstQuotStatusReq& lstReq) = 0;
        //������ѯBond����(������)��״̬
        virtual void QueryBondQuotByCodeStatus(const std::string& sReqID, const LstQuotStatusReq& lstReq) = 0;
        //��ϵ����Ϣ����
        virtual void ContactInfoPush(const LstPersonInfo& PersonInfos, const LstGroupInfo& GrpInfos) = 0;
        //��ϵ������״̬����
        virtual void ContactStatusPush(const LstContactStatus& lstStatus) = 0;
        //QBAccount������ѯQMID����
        virtual void QueryQMIDByQBAccountRet(const std::string& sMsgID, const LstQBAccount2QMID& lstRet) = 0;
        //��ѯ��ǰ���õ�QM���ѷ���
        virtual void QueryQMRemindConfigRet(const std::string& sMsgID, const QMRemindConfig& QMRCfg) = 0;
        //����QM�ؼ������ѷ���
        virtual void AddQMRemindKeyItemRet(const std::string& sMsgID, const std::string& ItemID) = 0;
        //�޸�QM�ؼ������ѷ���
        virtual void ModifyQMRemindKeyItemRet(const std::string& sMsgID, bool bSuccess) = 0;
        //ɾ��QM�ؼ������ѷ���
        virtual void DelQMRemindKeyItemRet(const std::string& sMsgID, const SetString& setSuccessID) = 0;
        //����QM�������ѷ���
        virtual void AddQMRemindFriendItemRet(const std::string& sMsgID, const std::string& ItemID) = 0;
        //�޸�QM�������ѷ���
        virtual void ModifyQMRemindFriendItemRet(const std::string& sMsgID, bool bSuccess) = 0;
        //ɾ��QM�������ѷ���
        virtual void DelQMRemindFriendItemRet(const std::string& sMsgID, const SetString& setSuccessID) = 0;
        //����QM���ѷ�ʽ����
        virtual void SetQMRemindTypeRet(const std::string& sMsgID, bool bSuccess) = 0;
        //������Ⱥgroup����
        virtual void ReqJoinGroupRet(const std::string& sMsgID, bool bSuccess) = 0;
        //����������
        virtual void ReqOpenNewsDetail(const NewsInfoV2 &info, bool bDisablePDF = true) = 0;
        //�����Ŵ���
        virtual void ReqOpenNewsWnd() = 0;
        //��ѯQM����״̬���� 1=����;-1=����;2=��¼��
        virtual void QueryQMStatusRet(int status) = 0;
        //QM�ǻ�����ϵ���޸ĺ�����ͽӿ�
        virtual void ModifyNoInstituteContactRet(const LstNoInstituteContact& Contacts) = 0;
	};

    struct QMProxyParam
    {
        //ProxyType:
        //Proxy_Type_None = 0,
        //Proxy_Type_HTTP = 1,
        //Proxy_Type_Socks4 = 2,
        //Proxy_Type_Socks5 = 3,
        //Proxy_Type_IE = 4,

        //��������
        int ProxyType;
        //�����ַ
        std::string ProxyIP;
        //����˿�
        std::string ProxyPort;
        //�����û�
        std::string ProxyUser;
        //��������
        std::string ProxyPwd;
        //��
        std::string ProxyDomain;

        QMProxyParam()
            :ProxyType(0)
            ,ProxyIP("")
            ,ProxyPort("")
            ,ProxyUser("")
            ,ProxyPwd("")
            ,ProxyDomain("")
        {
        }

        QMProxyParam& operator= (const QMProxyParam& pi)
        {
            ProxyType = pi.ProxyType;
            ProxyIP = pi.ProxyIP;
            ProxyPort = pi.ProxyPort;
            ProxyUser = pi.ProxyUser;
            ProxyPwd = pi.ProxyPwd;
            ProxyDomain = pi.ProxyDomain;
            return (*this) ;
        }
    };

	struct QBToQMInitParam
	{
		//�û�
		std::string sUsr;
		//����
		std::string sPwd;
		//QM����Ŀ¼
		std::string sQMDir;
		//QM����������
		std::string sQMExeName;
        //QM����������Ϣ
        QMProxyParam stQMProxyInfo;

        QBToQMInitParam()
            :sUsr("")
            ,sPwd("")
            ,sQMDir("")
            ,sQMExeName("")
        {}
	};

	class IQBToQM
	{
	protected:
		IQBToQM(){}

	public:
		virtual ~IQBToQM(){}

		//��ʼ��
		virtual bool Init(const QBToQMInitParam& InitParam) = 0;
		//ע��ص��ӿ�
		virtual void RegCallback(IQBToQMCallback* pCallback) = 0;
		//�ر����߼��������ӵĿͻ���(�����Լ�)
		virtual void CloseLocalDataBus() = 0;
		//ѡ����ϵ��
		virtual bool SelContacts(const std::string& sSelID, const std::string& sWndID = "", int nLimit = 200, bool bShowGrp = true) = 0;
		//�༭��ϵ��
		virtual bool EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID = "", int nLimit = 200, bool bShowGrp = true) = 0;
		//����/��������, ����MsgID���ں���Ӧƥ��
        virtual std::string SendQuot(const LstMMQuotSendInfoV2& lstSend) = 0;
		//��ѯ��ϵ����Ϣ(�첽����)
		virtual bool QueryContactInfo(const LstContact& lstIn) = 0;
		//�˳�QM bChkSessionΪtrueʱֻ�˳���ͬ�˺�,Ϊfalseʱ�����˺�,���˳�
		virtual bool QuitQM(bool bChkSession) = 0;
		//����ծȯ��Ϣ����
		virtual bool RequestBondInfosRet(const LstBondItem& LstRet) = 0;
		//��QM�����
		virtual bool OpenMainPanel(const std::string& sWndID = "") = 0;
		//�򿪶Ի�, bDialogue��ʾA���QB��Bͼ�귢��Ự��B�Ĵ����Ƿ��赯��
		virtual bool OpenDialogue(const Contact& ct, const std::string& sSendMsg, const std::string& sWndID = "", const bool bDialogue = true) = 0;
		//�ظ�����
		virtual bool ReplyQuot(const QuotReplyInfo& Reply, const std::string& sWndID = "") = 0;
		//��ѯ��ǰ�û���QMID
		virtual bool QueryCurUsrID() = 0;
		//��ѯ��ǰ�û�����ID
		virtual bool QueryCurUsrFriendIDs() = 0;
		//��ѯ��ǰ�û�ȺID
		virtual bool QueryCurUsrGroupIDs() = 0;
		//����/����ծȯ����(������), ����MsgID���ں���Ӧƥ��
		virtual std::string SendBondQuotByCode(const LstBondQuotByCodeSendInfoV2& lstSend) = 0;
		//�ظ�ծȯ����(������)
		virtual bool ReplyBondQuotByCode(const BondQuotByCodeReplyInfo& Reply, const std::string& sWndID = "") = 0;
		//��ָ���û�������Ϣ
		virtual bool SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID = "") = 0;
        //��ѯQM����λ��,ͬ���ӿ�,ע��timeout�ĵ�λ����,����IDΪ��ʱĬ�ϲ�ѯ������
        virtual bool QueryQMWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight, int timeout = -1) = 0;
        //��ѯQM���ھ��,ͬ���ӿ�,ע��timeout�ĵ�λ����,����IDΪ��ʱĬ�ϲ�ѯ������
        virtual bool QueryQMWndHandle(const std::string& sWndID, HWND& hWnd, int timeout = -1) = 0;
        //��ȡ�ظ���½��ʾͬ��״̬,������Ϊ0�򵯳���ʾ,����1�򲻵���ʾ
        virtual int GetMultiLoginStatus() = 0;
        //�����ظ���½��ʾͬ��״̬,��״̬��Ϊ0
        virtual void ResetMultiLoginStatus() = 0;
        //��ѯ��ǰ�û�������Ⱥ
        virtual bool QuerySharedGroup(Contact& ct, int timeout = -1) = 0;
        //����QM��ϵ��, ����MsgID���ں���Ӧƥ��
        virtual std::string SearchQMContact(const ContactSearch& Input) = 0;
        //��ѯ��ǰ�û���Ȩ�޿����Ľ���ԱQMID����
        virtual void GetAllDealerRet(const std::string& sReqID, const SetUInt64& setID) = 0;
        //��ָ���û������깺��Ϣ
        virtual std::string Purchase(const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID = "") = 0;
        //�깺״̬�ı�����
        virtual void PurchaseStatusPush(const MapPurchaseStatus& mapPStatus) = 0;
        //ȷ���깺����
        virtual void ConfirmPurchaseRet(const std::string& sReqID, int nStatus) = 0; 
        //�깺��ť�Ƿ���ʾ����
        virtual void ContactIsDealerPush(const Contact& ct, bool bRet) = 0;
        //�깺�༭��ť�Ƿ���ʾ����
        virtual void IsEditPurchaseValidPush(const Contact& ct, bool bRet) = 0;
        //������ѯMM����״̬����
        virtual void QueryMMQuotStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet) = 0;
        //������ѯBond����(������)��״̬����
        virtual void QueryBondQuotByCodeStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet) = 0;
        //ͨ��QBAccount������ѯQMID, ����MsgID���ں���Ӧƥ��
        virtual std::string QueryQMIDByQBAccount(const SetString& setQBAccount) = 0;
        //��ѯ��ǰ���õ�QM����, ����MsgID���ں���Ӧƥ��
        virtual std::string QueryQMRemindConfig() = 0;
        //����QM�ؼ�������, ����MsgID���ں���Ӧƥ��
        virtual std::string AddQMRemindKeyItem(const QMRemindKeyItem& Item) = 0;
        //�޸�QM�ؼ�������, ����MsgID���ں���Ӧƥ��
        virtual std::string ModifyQMRemindKeyItem(const QMRemindKeyItem& Item) = 0;
        //ɾ��QM�ؼ�������, ����MsgID���ں���Ӧƥ��
        virtual std::string DelQMRemindKeyItem(const SetString& setID) = 0;
        //����QM��������, ����MsgID���ں���Ӧƥ��
        virtual std::string AddQMRemindFriendItem(const QMRemindFriendItem& Item) = 0;
        //�޸�QM��������, ����MsgID���ں���Ӧƥ��
        virtual std::string ModifyQMRemindFriendItem(const QMRemindFriendItem& Item) = 0;
        //ɾ��QM��������, ����MsgID���ں���Ӧƥ��
        virtual std::string DelQMRemindFriendItem(const SetString& setID) = 0;
        //����QM���ѷ�ʽ, ����MsgID���ں���Ӧƥ��
        virtual std::string SetQMRemindType(unsigned int nRemindType) = 0;
        //��������Ѷ
        virtual bool OpenNewsBulletin(const std::string& sWndID = "") = 0;
        //��ѯ�ͷ�
        virtual bool QueryCustomService(Contact& ct, int timeout = -1) = 0;
        //QB�޸�����
        virtual void QBModifyPwd(const std::string& sNewPwd) = 0;
        //��ָ���û�����ծȯ
        virtual bool ShareBond(const LstContact& Contacts, const LstBondShare& lstBS, const std::string& sWndID = "") = 0;
        //������Ⱥgroup
        virtual std::string ReqJoinGroup(const unsigned long long &llGroup) = 0;
		//��ָ������
		virtual void ReqOpenNews(const NewsInfo &info) = 0;
        //����ͼƬ
        virtual void SharePic(const PicInfo &info, const LstContact& Contacts) = 0;
        //�������Ÿ�ָ����ϵ��
        virtual void ShareNewsDetail(const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF = true) = 0;
        //����QB userID �򿪶Ի���
        virtual void OpenDialogueByUserID(const std::string &userID) = 0;
        //��ѯQM����״̬
        virtual void QueryQMStatus() = 0;
        //����QM�����̴��ڽӿ�
        virtual void ReqOpenSellsWnd(const std::string& sWndID = "") = 0;
        //��ѯQM�Ǻ���������ϵ�˽ӿ� status��ʾ��ѯ�Ƿ�ɹ� (0=�ɹ�;1=����)
        virtual void ReqQueryNoInstituteContact(LstNoInstituteContact &contacts, int &status, int timeout = -1) = 0;
        //����QM�Ǻ���������ϵ�˽ӿ�
        virtual void ReqSetNoInstituteContact(const std::list<std::string> &QBAccounts) = 0;
        //����QM�����̽����еĵ������󵯳��Ĳ��Ҵ���
        virtual void ReqOpenSellsSearchWnd(const std::string& sWndID = "") = 0;
        //QB�û��������,֪ͨQM
        virtual void ReqOpenDialogueFromAd(const std::string &userID) = 0;
	};
}

QB_QM_API QB_QM::IQBToQM* GetQBToQM();

