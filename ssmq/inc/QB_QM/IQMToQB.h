#pragma once

#include "QB_QMDefine.h"

//�ܵ������ӿ�,�ṩ������صķ���

namespace QB_QM
{
	//MM��ص������ӿ�,ʹ�õĵط�����"IQMToQB_MM.h"
	class IQMToQB_MM;
	//Bond��ص������ӿ�,ʹ�õĵط�����"IQMToQB_Bond.h"
	class IQMToQB_Bond;
    //�깺��ص������ӿ�,ʹ�õĵط�����"IQMToQB_Purchase.h"
    class IQMToQB_Purchase;
    //QM������ص������ӿ�,ʹ�õĵط�����"IQMToQB_Remind.h"
    class IQMToQB_Remind;

	//�ص��ӿ�,��QMʵ��
	class IQMToQBCallback
	{
	protected:
		IQMToQBCallback(){}

	public:
		virtual ~IQMToQBCallback(){}

		//ѡ����ϵ��
		virtual bool SelContacts(const std::string& sSelID, const std::string& sWndID, int nLimit, bool bShowGrp) = 0;
		//�༭��ϵ��
		virtual bool EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID, int nLimit, bool bShowGrp) = 0;
		//��ѯ��ϵ����Ϣ,��ѯʱ��Ч��ID��lstFailed����
		virtual bool QueryContactInfo(const LstContact& lstIn, LstPersonInfo& lstPerson, LstGroupInfo& lstGrp, LstContact& lstFailed) = 0;
		//�˳�QM
		virtual void QuitQM() = 0;
		//��QM�����
		virtual bool OpenMainPanel(const std::string& sWndID) = 0;
		//�򿪶Ի�
		virtual bool OpenDialogue(const Contact& ct, const std::string& sSendMsg, const std::string& sWndID, const bool bDialogue = true) = 0;
		//��ȡ��ǰ�û���QMID
		virtual unsigned long long GetCurUsrID() = 0;
		//��ѯ��ǰ�û�����ID
		virtual bool QueryCurUsrFriendIDs() = 0;
		//��ѯ��ǰ�û�ȺID
		virtual bool QueryCurUsrGroupIDs() = 0;
		//ͨ��QB״̬ 1:QB�������� 0:QB����������
		virtual void QBStatus(int nStatus) = 0;
		//��ָ���û�������Ϣ
		virtual bool SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID) = 0;
        //��ѯQM����λ��,��sWndIDΪ����Ĭ��Ϊ������λ��
        virtual bool QueryQMWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight) = 0;
        //��ѯQM���ھ��,��sWndIDΪ����Ĭ��Ϊ�����ھ��
        virtual bool QueryQMWndHandle(const std::string& sWndID, unsigned long long& hWnd) = 0;
        //��ѯ��ǰ�û�������Ⱥ
        virtual bool QuerySharedGroup(Contact& ct) = 0;
        //����QM��ϵ��
        virtual bool SearchQMContact(const ContactSearch& Input, LstSearchUserInfo& Result) = 0;
        //QBAccount������ѯQMID, MsgID���ں���Ӧƥ��
        virtual void QueryQMIDByQBAccount(const std::string& sMsgID, const SetString& setQBAccount) = 0;
        //��������Ѷ
        virtual bool OpenNewsBulletin(const std::string& sWndID) = 0;
        //��ѯ�ͷ�
        virtual bool QueryCustomService(Contact& ct) = 0;
        //QB�޸�����
        virtual void QBModifyPwd(const std::string& sNewPwd) = 0;
        //������Ⱥgroup
        virtual void ReqJoinGroup(const std::string& sMsgID, const unsigned long long &llGroup) = 0;
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
        virtual void ReqOpenSellsWnd(const std::string& sWndID) = 0;
        //��ѯQM�Ǻ���������ϵ�˽ӿ�
        virtual void ReqQueryNoInstituteContact(LstNoInstituteContact &contacts, int &status) = 0;
        //����QM�Ǻ���������ϵ�˽ӿ�
        virtual void ReqSetNoInstituteContact(const std::list<std::string> &QBAccounts) = 0;
        //����QM�����̽����еĵ������󵯳��Ĳ��Ҵ���
        virtual void ReqOpenSellsSearchWnd(const std::string& sWndID) = 0;
        //QB�û��������,֪ͨQM
        virtual void ReqOpenDialogueFromAd(const std::string &userID) = 0;
	};

	class IQMToQB
	{
	protected:
		IQMToQB(){}

	public:
		virtual ~IQMToQB(){}
		
		//��ʼ��
		virtual void Init(const std::string& sUsr, const std::string& sPwd) = 0;
		//�ͷ�ǰ������,QM�˳�ǰ����
		virtual void Release() = 0;
		//ע��ص��ӿ�
		virtual void RegCallback(IQMToQBCallback* pCallback) = 0;
        //�ر����߼��������ӵĿͻ���(�����Լ�)
        virtual void CloseLocalDataBus() = 0;
		//��ȡMM��������ӿ�
		virtual IQMToQB_MM* GetSubInterface_MM() = 0;
		//��ȡBond��������ӿ�
		virtual IQMToQB_Bond* GetSubInterface_Bond() = 0;
        //��ȡ�깺��������ӿ�
        virtual IQMToQB_Purchase* GetSubInterface_Purchase() = 0;
        //��ȡQM������������ӿ�
        virtual IQMToQB_Remind* GetSubInterface_Remind() = 0;
		//ѡ����ϵ�˷���
		virtual bool SelContactsRet(const std::string& sSelID, const LstContact& Contacts) = 0;
		//�༭��ϵ�˷���
		virtual bool EditContactsRet(const std::string& sEditID, const LstContact& Contacts) = 0;
		//����QM��״̬ 1Ϊ����;-1Ϊ����;2Ϊ��½�� 
		virtual void SetStatus(int nStatus) = 0;
		//��ѯ��ǰ�û�����ID����
		virtual void QueryCurUsrFriendIDsRet(const SetUInt64& setIDs) = 0;
		//��ѯ��ǰ�û�ȺID����
		virtual void QueryCurUsrGroupIDsRet(const SetUInt64& setIDs) = 0;
		//��ǰ�û�Ⱥ�仯ʱ֪ͨQB    JoinGrpIDs:�¼����ȺID    LeaveGrpIDs:�뿪��ȺID
		virtual void CurUsrGroupChange(const SetUInt64& JoinGrpIDs, const SetUInt64& LeaveGrpIDs) = 0;
        //��ǰ�û����ѱ仯ʱ֪ͨQB    AddFriendIDs:�¼Ӻ���ID    DelFriendIDs:ɾ������ID
        virtual void CurUsrFriendChange(const SetUInt64& AddFriendIDs, const SetUInt64& DelFriendIDs) = 0;
        //��ѯQB����λ��,ͬ���ӿ�,ע��timeout�ĵ�λ����,����IDΪ��ʱĬ�ϲ�ѯ������
        virtual bool QueryQBWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight, int timeout = -1) = 0;
        //��ѯQB���ھ��,ͬ���ӿ�,ע��timeout�ĵ�λ����,����IDΪ��ʱĬ�ϲ�ѯ������
        virtual bool QueryQBWndHandle(const std::string& sWndID, unsigned long long& hWnd, int timeout = -1) = 0;
        //��ѯQB���Ȩ��,ͬ���ӿ�,ע��timeout�ĵ�λ����
        //Ŀǰ֧�ֵ�Ȩ��:MM����--mmquot;
        virtual bool QueryQBPerm(SetString& setPerm, int timeout = -1) = 0;
        //��ȡ�ظ���½��ʾͬ��״̬,������Ϊ0�򵯳���ʾ,����1�򲻵���ʾ
        virtual int GetMultiLoginStatus() = 0;
        //�����ظ���½��ʾͬ��״̬,��״̬��Ϊ0
        virtual void ResetMultiLoginStatus() = 0;
        //����QM������Ϣ
        virtual void QMErrInfo(int nErrCode, const std::string& sErrInfo) = 0;
        //��ϵ����Ϣ����
        virtual void ContactInfoPush(const LstPersonInfo& PersonInfos, const LstGroupInfo& GrpInfos) = 0;
        //��ϵ������״̬����
        virtual void ContactStatusPush(const LstContactStatus& lstStatus) = 0;
        //QBAccount������ѯQMID����
        virtual void QueryQMIDByQBAccountRet(const std::string& sMsgID, const LstQBAccount2QMID& lstRet) = 0;
        //������Ⱥgroup����
        virtual void ReqJoinGroupRet(const std::string& sMsgID, bool bSuccess) = 0;
        //����������,bDisablePDF��ʾ�Ƿ����PDF����
        virtual void ReqOpenNewsDetail(const NewsInfoV2 &info, bool bDisablePDF = true) = 0;
        //�����Ŵ���
        virtual void ReqOpenNewsWnd() = 0;
        //��ѯQM����״̬���� 1=����;-1=����;2=��¼��
        virtual void QueryQMStatusRet(int status) = 0;
        //QM�ǻ�����ϵ���޸ĺ�����ͽӿ�
        virtual void ModifyNoInstituteContactRet(const LstNoInstituteContact& Contacts) = 0;
	};
}

QB_QM_API QB_QM::IQMToQB* GetQMToQB();

