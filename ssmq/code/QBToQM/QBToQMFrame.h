#pragma once
#include "IQBToQM.h"
#include "IQBCommFrame.h"
#include "SSTool.h"
#include "BaseStringFunc.h"
#include "../sdbus/message.h"
#include <boost/thread.hpp>
#include "ISSMQMng.h"

using namespace QB_QM;

namespace ss
{
	class logger;
}

class CQBObs_Base;
typedef std::list<CQBObs_Base*> LstObs;

class CQBToQMFrame : public IQBToQM, 
					 public ss_tool::SingletonTmpl<CQBToQMFrame>, 
					 public IQBCommFrame,
					 public SSMQ::ISSMQMngCallback
{
	friend class ss_tool::SingletonTmpl<CQBToQMFrame>;
protected:
	CQBToQMFrame(void);
public:
	virtual ~CQBToQMFrame(void);

	//Interface of ISSMQMngCallback
	void CloseClient();

	//Interface of IQBToQM
	bool Init(const QBToQMInitParam& InitParam);
	void RegCallback(IQBToQMCallback* pCallback);
	void CloseLocalDataBus();
	bool SelContacts(const std::string& sSelID, const std::string& sWndID = "", int nLimit = 200, bool bShowGrp = true);
	bool EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID = "", int nLimit = 200, bool bShowGrp = true);
	bool SendQuot(const LstQuotSendInfo& lstSend);
    std::string  SendQuot(const LstMMQuotSendInfoV2& lstSend);
	bool QueryContactInfo(const LstContact& lstIn);
	bool QuitQM(bool bChkSession);
	bool RequestBondInfosRet(const LstBondItem& LstRet);
	bool OpenMainPanel(const std::string& sWndID = "");
	//bool RefreshBondInfos();
	bool OpenDialogue(const Contact& ct, const string& sSendMsg, const std::string& sWndID = "", const bool bOpen = true);
	bool ReplyQuot(const QuotReplyInfo& Reply, const std::string& sWndID = "");
	bool QueryCurUsrID();
	bool QueryCurUsrFriendIDs();
	bool QueryCurUsrGroupIDs();
    bool SendBondQuotByCode(const LstBondQuotByCodeSendInfo& lstSend);
    std::string SendBondQuotByCode(const LstBondQuotByCodeSendInfoV2& lstSend);
    bool SendBondQuotByCondition(const LstBondQuotByConditionSendInfo& lstSend);
    bool ReplyBondQuotByCode(const BondQuotByCodeReplyInfo& Reply, const std::string& sWndID = "");
	bool SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID = "");
    bool QueryQMWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight, int timeout = -1);
    bool QueryQMWndHandle(const std::string& sWndID, HWND& hwnd, int timeout = -1);
    int GetMultiLoginStatus();
    void ResetMultiLoginStatus();
    bool QuerySharedGroup(Contact& ct, int timeout = -1);
    std::string SearchQMContact(const ContactSearch& Input);
    void GetAllDealerRet(const std::string& sReqID, const SetUInt64& setID);
    std::string Purchase(const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID = "");
    void PurchaseStatusPush(const MapPurchaseStatus& mapPStatus);
    void ConfirmPurchaseRet(const std::string& sReqID, int nStatus); 
    void ContactIsDealerPush(const Contact& ct, bool bRet);
    void IsEditPurchaseValidPush(const Contact& ct, bool bRet);
    void QueryMMQuotStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet);
    void QueryBondQuotByCodeStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet);
    std::string QueryQMIDByQBAccount(const SetString& setQBAccount);
    std::string QueryQMRemindConfig();
    std::string AddQMRemindKeyItem(const QMRemindKeyItem& Item);
    std::string ModifyQMRemindKeyItem(const QMRemindKeyItem& Item);
    std::string DelQMRemindKeyItem(const SetString& setID);
    std::string AddQMRemindFriendItem(const QMRemindFriendItem& Item);
    std::string ModifyQMRemindFriendItem(const QMRemindFriendItem& Item);
    std::string DelQMRemindFriendItem(const SetString& setID);
    std::string SetQMRemindType(unsigned int nRemindType);
    bool OpenNewsBulletin(const std::string& sWndID = "");
    bool QueryCustomService(Contact& ct, int timeout = -1);
    void QBModifyPwd(const std::string& sNewPwd);
    bool ShareBond(const LstContact& Contacts, const LstBondShare& lstBS, const std::string& sWndID = "");
    std::string ReqJoinGroup(const unsigned long long &llGroup);
	virtual void ReqOpenNews(const NewsInfo &info);
    virtual void SharePic(const PicInfo &info, const LstContact& Contacts);
    virtual void ShareNewsDetail(const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF = true);
    virtual void OpenDialogueByUserID(const std::string &userID);
    virtual void QueryQMStatus();
    virtual void ReqOpenSellsWnd(const std::string& sWndID = "");
    virtual void ReqQueryNoInstituteContact(LstNoInstituteContact &contacts, int &status, int timeout = -1);
    virtual void ReqSetNoInstituteContact(const std::list<std::string> &QBAccounts);
    virtual void ReqOpenSellsSearchWnd(const std::string& sWndID = "");
    virtual void ReqOpenDialogueFromAd(const std::string &userID);

	//Interface of IQBCommFrame
	void SetQMStatus(int nStatus, string sSessionID);
	string GetSessionID();
	bool SelContactsRet(const std::string& sSelID, const LstContact& Contacts);
	bool QueryContactInfoRet(LstPersonInfo& PersonInfos, LstGroupInfo& GrpInfos, const LstContact& lstFailed);
	bool EditContactsRet(const std::string& sEditID, const LstContact& Contacts);
	bool SendQuotRet(LstQuotSendInfo& lstSuccess, LstQuotSendInfo& lstFailed);
	bool RequestBondInfos();
	bool ShowDlgBondDetail(const std::string& sBondKey);
	bool RecvQuot(const LstQuotRecvInfo& lstRecv);
	void QueryCurUsrIDRet(unsigned long long UsrID);
	bool SendBondQuotByCodeRet(const LstBondQuotByCodeSendInfo& lstSuccess, const LstBondQuotByCodeSendInfo& lstFailed);
	bool SendBondQuotByConditionRet(const LstBondQuotByConditionSendInfo& lstSuccess, const LstBondQuotByConditionSendInfo& lstFailed);
	void QueryCurUsrFriendIDsRet(const SetUInt64& setIDs);
	void QueryCurUsrGroupIDsRet(const SetUInt64& setIDs);
	void CurUsrGroupChange(const SetUInt64& JoinGrpIDs, const SetUInt64& LeaveGrpIDs);
    void CurUsrFriendChange(const SetUInt64& AddFriendIDs, const SetUInt64& DelFriendIDs);
	void GroupDisband(unsigned long long GrpID);
    bool QueryQBWndPos(const std::string& sMsgID, const std::string& sWndID);
    bool QueryQBWndHandle(const std::string& sMsgID, const std::string& sWndID);
    bool QueryQBPerm(const std::string& sMsgID);
    void OpenMMQuot(const LstContact& Contacts, const std::string& sWndID);
    void SearchQMContactRet(const std::string& sMsgID, const LstSearchUserInfo& Result);
    void QueryContactIsDealer(const std::string& sMsgID, const Contact& ct);
    void OpenSubscribeDlg(const std::string& sMsgID, const Contact& ct, const std::string& sWndID);
    void GetAllDealer(const std::string& sMsgID);
    bool SendMMQuotRetV2(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed);
    bool SendBondQuotByCodeRetV2(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed);
    void QMErrInfo(int nErrCode, const std::string& sErrInfo);
    void QueryPurchaseStatus(const std::string& sMsgID, const LstPurchaseUkey& lstQuery);
    void ConfirmPurchase(const std::string& sMsgID, const std::string& sPurchaseID, time_t ModifyTime); 
    void IsEditPurchaseValid(const std::string& sMsgID, const Contact& ct);
    void EditPurchase(const std::string& sMsgID, const Contact& ct, const std::string& sWndID);
    void QueryMMQuotStatus(const std::string& sMsgID, const LstQuotStatusReq& lstReq);
    void QueryBondQuotByCodeStatus(const std::string& sMsgID, const LstQuotStatusReq& lstReq);
    void ContactInfoPush(const std::string& sMsgID, const LstPersonInfo& PersonInfos, const LstGroupInfo& GrpInfos);
    void ContactStatusPush(const std::string& sMsgID, const LstContactStatus& lstStatus);
    void QueryQMIDByQBAccountRet(const std::string& sMsgID, const LstQBAccount2QMID& lstRet);
    void QueryQMRemindConfigRet(const std::string& sMsgID, const QMRemindConfig& QMRCfg);
    void AddQMRemindKeyItemRet(const std::string& sMsgID, const std::string& ItemID);
    void ModifyQMRemindKeyItemRet(const std::string& sMsgID, bool bSuccess);
    void DelQMRemindKeyItemRet(const std::string& sMsgID, const SetString& setSuccessID);
    void AddQMRemindFriendItemRet(const std::string& sMsgID, const std::string& ItemID);
    void ModifyQMRemindFriendItemRet(const std::string& sMsgID, bool bSuccess);
    void DelQMRemindFriendItemRet(const std::string& sMsgID, const SetString& setSuccessID);
    void SetQMRemindTypeRet(const std::string& sMsgID, bool bSuccess);
    void ReqJoinGroupRet(const std::string& sMsgID, bool bSuccess);
    virtual void ReqOpenNewsDetail(const NewsInfoV2 &info, bool bDisablePDF = true);
    virtual void ReqOpenNewsWnd();
    virtual void QueryQMStatusRet(int status);
    virtual void ModifyNoInstituteContactRet(const LstNoInstituteContact& Contacts);

	//等待心跳超时
	bool WaitforHeartbeat();
	//心跳超时处理
	void HBTimeout();

protected:
	void InitObs();
	void ClearObs();
	void StartQM();
	bool WaitForRightStatus();
	string GetSendQueueID(const sdbus::Message& msgs);
	int FindProcessByName(t_string sFileName);
	t_string GetModuleDirPath();
	void StartGuardThread();
	void StopGuardThread();

	//Heartbeat
	void Heartbeat();
	void HeartbeatThread();
	void StartHeartbeat();
	void StopHeartbeat();

public:
	volatile bool m_bStopGuard;
	HANDLE m_hHeartbeatEvent;

private:
	string m_sUsr;
	string m_sPwd;
    QMProxyParam m_stQMProxyInfo;
	LstObs m_LstObs;
	int m_nState;
	IQBToQMCallback* m_pCallback;
	t_string m_sQMDir;
	t_string m_sQMExeName;
	int m_nQMStatus;
	HANDLE m_hGuardThread;
	ss::logger* m_pLogger;

    HANDLE m_hMultiLoginMutex;

	boost::thread* m_ThreadHeartbeat;
	bool m_bStopHeartbeat;
};
