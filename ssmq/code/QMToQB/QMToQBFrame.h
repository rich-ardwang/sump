#pragma once
#include "IQMToQB.h"
#include "IQMToQB_MM.h"
#include "IQMToQB_Bond.h"
#include "IQMToQB_Purchase.h"
#include "IQMToQB_Remind.h"
#include "IQMCommFrame.h"
#include "SSTool.h"
#include "../sdbus/message.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "ISSMQMng.h"

using namespace QB_QM;

namespace ss
{
	class logger;
}

class CQMObs_Base;
typedef std::list<CQMObs_Base*> LstObs;

class CQMToQBFrame : public IQMToQB, 
					 public IQMToQB_MM, 
					 public IQMToQB_Bond,
                     public IQMToQB_Purchase,
                     public IQMToQB_Remind,
					 public ss_tool::SingletonTmpl<CQMToQBFrame>, 
					 public IQMCommFrame, 
					 public SSMQ::ISSMQMngCallback
{
	friend class ss_tool::SingletonTmpl<CQMToQBFrame>;
protected:
	CQMToQBFrame(void);
public:
	virtual ~CQMToQBFrame(void);

	void Heartbeat();

	//Interface of ISSMQMngCallback
	void CloseClient();

	//Interface of IQMToQB
	void Init(const std::string& sUsr, const std::string& sPwd);
	void Release();
	void RegCallback(IQMToQBCallback* pCallback);
    void CloseLocalDataBus();
	IQMToQB_MM* GetSubInterface_MM();
	IQMToQB_Bond* GetSubInterface_Bond();
    IQMToQB_Purchase* GetSubInterface_Purchase();
    IQMToQB_Remind* GetSubInterface_Remind();
	bool SelContactsRet(const std::string& sSelID, const LstContact& Contacts);
	bool EditContactsRet(const std::string& sEditID, const LstContact& Contacts);
	void SetStatus(int nStatus);
	void QueryCurUsrFriendIDsRet(const SetUInt64& setIDs);
	void QueryCurUsrGroupIDsRet(const SetUInt64& setIDs);
	void CurUsrGroupChange(const SetUInt64& JoinGrpIDs, const SetUInt64& LeaveGrpIDs);
    void CurUsrFriendChange(const SetUInt64& AddFriendIDs, const SetUInt64& DelFriendIDs);
	void GroupDisband(unsigned long long GrpID);
    bool QueryQBWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight, int timeout = -1);
    bool QueryQBWndHandle(const std::string& sWndID, unsigned long long& hWnd, int timeout = -1);
    bool QueryQBPerm(SetString& setPerm, int timeout = -1);
    void OpenMMQuot(const LstContact& Contacts, const std::string& sWndID = "");
    int GetMultiLoginStatus();
    void ResetMultiLoginStatus();
    void QMErrInfo(int nErrCode, const std::string& sErrInfo);
    void ContactInfoPush(const LstPersonInfo& PersonInfos, const LstGroupInfo& GrpInfos);
    void ContactStatusPush(const LstContactStatus& lstStatus);
    void QueryQMIDByQBAccountRet(const std::string& sMsgID, const LstQBAccount2QMID& lstRet);
    void ReqJoinGroupRet(const std::string& sMsgID, bool bSuccess);
    virtual void ReqOpenNewsDetail(const NewsInfoV2 &info, bool bDisablePDF = true);
    virtual void ReqOpenNewsWnd();
    virtual void QueryQMStatusRet(int status);
    virtual void ModifyNoInstituteContactRet(const LstNoInstituteContact& Contacts);

	//Interface of IQMToQB_MM
	void RegCallback(IQMToQBCallback_MM* pCallback);
	bool SendQuotRet(LstQuotSendInfo& lstSuccess, LstQuotSendInfo& lstFailed);
    bool SendQuotRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed);
    std::string QueryMMQuotStatus(const LstQuotStatusReq& lstReq);

	//Interface of IQMToQB_Bond
	void RegCallback(IQMToQBCallback_Bond* pCallback);
	bool RequestBondInfos();
	bool ShowDlgBondDetail(const std::string& sBondKey);
	bool SendBondQuotByCodeRet(const LstBondQuotByCodeSendInfo& lstSuccess, const LstBondQuotByCodeSendInfo& lstFailed);
    bool SendBondQuotByCodeRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed);
	bool SendBondQuotByConditionRet(const LstBondQuotByConditionSendInfo& lstSuccess, const LstBondQuotByConditionSendInfo& lstFailed);
    std::string QueryBondQuotByCodeStatus(const LstQuotStatusReq& lstReq);

    //Interface of IQMToQB_Purchase
    void RegCallback(IQMToQBCallback_Purchase* pCallback);
    std::string ContactIsDealer(const Contact& ct);
    std::string GetAllDealer();
    void OpenSubscribeDlg(const Contact& ct, const std::string& sWndID = "");
    std::string QueryPurchaseStatus(const LstPurchaseUkey& lstQuery);
    std::string ConfirmPurchase(const std::string& sPurchaseID, time_t ModifyTime); 
    std::string IsEditPurchaseValid(const Contact& ct);
    void EditPurchase(const Contact& ct, const std::string& sWndID = "");

    //Interface of IQMToQB_Remind
    void RegCallback(IQMToQBCallback_Remind* pCallback);
    void QueryQMRemindConfigRet(const string& sMsgID, const QMRemindConfig& QMRCfg);
    void AddQMRemindKeyItemRet(const string& sMsgID, const string& ItemID);
    void ModifyQMRemindKeyItemRet(const string& sMsgID, bool bSuccess);
    void DelQMRemindKeyItemRet(const string& sMsgID, const SetString& setSuccessID);
    void AddQMRemindFriendItemRet(const string& sMsgID, const string& ItemID);
    void ModifyQMRemindFriendItemRet(const string& sMsgID, bool bSuccess);
    void DelQMRemindFriendItemRet(const string& sMsgID, const SetString& setSuccessID);
    void SetQMRemindTypeRet(const string& sMsgID, bool bSuccess);

	//Interface of IQMCommFrame
	void Quit(bool bChkSession, const std::string& sSessionID);
	string GetSessionID();
	void SelContacts(const std::string& sSelID, const std::string& sWndID, int nLimit, bool bShowGrp);
	void EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID, int nLimit, bool bShowGrp);
	void SetQBStatus(int nStatus, string sSessionID);
	void SendQuot(const LstQuotSendInfo& lstSend);
    void QueryContactInfo(const std::string& sMsgID, const LstContact& lstIn);
	void RequestBondInfosRet(const LstBondItem& LstRet);
	void OpenMainPanel(const std::string& sWndID);
	bool RefreshBondInfos();
	bool OpenDialogue(const Contact& ct, const std::string& sSendMsg, const std::string& sWndID, const bool bOpen);
	bool SendBondQuotByCode(const LstBondQuotByCodeSendInfo& lstSend);
	bool SendBondQuotByCondition(const LstBondQuotByConditionSendInfo& lstSend);
	bool ReplyQuot(const QuotReplyInfo& Reply, const std::string& sWndID);
	void QueryCurUsrID();
	bool QueryCurUsrFriendIDs();
	bool QueryCurUsrGroupIDs();
	bool SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID);
    bool QueryQMWndPos(const std::string& sMsgID, const std::string& sWndID);
    bool QueryQMWndHandle(const std::string& sMsgID, const std::string& sWndID);
    bool QuerySharedGroup(const std::string& sMsgID);
    bool SearchQMContact(const std::string& sMsgID, const ContactSearch& Input);
	bool ReplyBondQuotByCode(const BondQuotByCodeReplyInfo& Reply, const std::string& sWndID);
    void SendQuotV2(const std::string& sMsgID, const LstMMQuotSendInfoV2& lstSend);
    void SendBondQuotByCodeV2(const std::string& sMsgID, const LstBondQuotByCodeSendInfoV2& lstSend);
    void Purchase(const std::string& sMsgID, const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID);
    void ContactIsDealerRet(const std::string& sMsgID, bool bRet);
    void GetAllDealerRet(const std::string& sMsgID, const SetUInt64& setID);
    void QueryPurchaseStatusRet(const std::string& sMsgID, const MapPurchaseStatus& mapPStatus);
    void ConfirmPurchaseRet(const std::string& sMsgID, int nStatus);
    void IsEditPurchaseValidRet(const std::string& sMsgID, bool bRet);
    void PurchaseStatusPush(const std::string& sMsgID, const MapPurchaseStatus& mapPStatus);
    void QueryMMQuotStatusRet(const std::string& sMsgID, const LstQuotStatus& lstRet);
    void QueryBondQuotByCodeStatusRet(const std::string& sMsgID, const LstQuotStatus& lstRet);
    void ContactIsDealerPush(const std::string& sMsgID, const Contact& ct, bool bRet);
    void IsEditPurchaseValidPush(const std::string& sMsgID, const Contact& ct, bool bRet);
    void QueryQMIDByQBAccount(const std::string& sMsgID, const SetString& setQBAccount);
    void QueryQMRemindConfig(const string& sMsgID);
    void AddQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item);
    void ModifyQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item);
    void DelQMRemindKeyItem(const string& sMsgID, const SetString& setID);
    void AddQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item);
    void ModifyQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item);
    void DelQMRemindFriendItem(const string& sMsgID, const SetString& setID);
    void SetQMRemindType(const string& sMsgID, unsigned int nRemindType);
    void OpenNewsBulletin(const std::string& sWndID);
    bool QueryCustomService(const string& sMsgID);
    void QBModifyPwd(const std::string& sNewPwd);
    bool ShareBond(const LstContact& Contacts, const LstBondShare& lstBS, const std::string& sWndID);
    bool ReqJoinGroup(const std::string& sMsgID, const unsigned long long &llGroup);
	virtual void ReqOpenNews(const NewsInfo &info);
    virtual void SharePic(const PicInfo &info, const LstContact& Contacts);
    virtual void ShareNewsDetail(const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF = true);
    virtual void OpenDialogueByUserID(const std::string &userID);
    virtual void QueryQMStatus();
    virtual void ReqOpenSellsWnd(const std::string& sWndID);
    virtual void ReqQueryNoInstituteContact(const std::string& sMsgID);
    virtual void ReqSetNoInstituteContact(const std::list<std::string> &QBAccounts);
    virtual void ReqOpenSellsSearchWnd(const std::string& sWndID);
    virtual void ReqOpenDialogueFromAd(const std::string &userID);

protected:
	void StartHeartbeat();
	void StopHeartbeat();
	void InitObs();
	void ClearObs();
	string GetSendQueueID(const sdbus::Message& msgs);

	//Guard Thread 监听QB的心跳
	void GuardThread();
	void StartGuardThread();
	void StopGuardThread();
	//等待心跳超时
	bool WaitforHeartbeat();
	//心跳超时处理
	void HBTimeout();

public:
	volatile bool m_bStopHeartbeat;

private:
	string m_sUsr;
	string m_sPwd;
	LstObs m_LstObs;
	HANDLE m_HeartbeatThread;
	int m_nState;
	ss::logger* m_pLogger;
	IQMToQBCallback* m_pCallback;
	IQMToQBCallback_MM* m_pCallback_MM;
	IQMToQBCallback_Bond* m_pCallback_Bond;
    IQMToQBCallback_Purchase* m_pCallback_Purchase;
    IQMToQBCallback_Remind* m_pCallback_Remind;

	volatile bool m_bStopGuardThread;
	boost::thread* m_GuardThread;
	int m_nQBStatus;
	boost::mutex m_mutex;
	boost::condition m_conditionHB;

    HANDLE m_hMultiLoginMutex;
};
