#pragma once
#include "QB_QMDefine.h"
using std::string;
using namespace QB_QM;

class IQMCommFrame
{
protected:
	IQMCommFrame(){}
public:
	virtual ~IQMCommFrame(){}

	virtual void Quit(bool bChkSession, const std::string& sSessionID) = 0;
	virtual string GetSessionID() = 0;
	virtual void SelContacts(const std::string& sSelID, const std::string& sWndID, int nLimit, bool bShowGrp) = 0;
	virtual void EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID, int nLimit, bool bShowGrp) = 0;
	virtual	void SetQBStatus(int nStatus, string sSessionID) = 0;
	virtual void SendQuot(const LstQuotSendInfo& lstSend) = 0;
	virtual void QueryContactInfo(const std::string& sMsgID, const LstContact& lstIn) = 0;
	virtual void RequestBondInfosRet(const LstBondItem& LstRet) = 0;
	virtual void OpenMainPanel(const std::string& sWndID) = 0;
	virtual bool RefreshBondInfos() = 0;
	virtual bool OpenDialogue(const Contact& ct, const std::string& sSendMsg, const std::string& sWndID, const bool bDialogue = true) = 0;
	virtual bool SendBondQuotByCode(const LstBondQuotByCodeSendInfo& lstSend) = 0;
	virtual bool SendBondQuotByCondition(const LstBondQuotByConditionSendInfo& lstSend) = 0;
	virtual bool ReplyQuot(const QuotReplyInfo& Reply, const std::string& sWndID) = 0;
	virtual void QueryCurUsrID() = 0;
	virtual bool QueryCurUsrFriendIDs() = 0;
	virtual bool QueryCurUsrGroupIDs() = 0;
	virtual bool SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID) = 0;
    virtual bool QueryQMWndPos(const std::string& sMsgID, const std::string& sWndID) = 0;
    virtual bool QueryQMWndHandle(const std::string& sMsgID, const std::string& sWndID) = 0;
    virtual bool QuerySharedGroup(const std::string& sMsgID) = 0;
    virtual bool SearchQMContact(const std::string& sMsgID, const ContactSearch& Input) = 0;
	virtual bool ReplyBondQuotByCode(const BondQuotByCodeReplyInfo& Reply, const std::string& sWndID) = 0;
    virtual void SendQuotV2(const std::string& sMsgID, const LstMMQuotSendInfoV2& lstSend) = 0;
    virtual void SendBondQuotByCodeV2(const std::string& sMsgID, const LstBondQuotByCodeSendInfoV2& lstSend) = 0;
    virtual void Purchase(const std::string& sMsgID, const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID) = 0;
    virtual void ContactIsDealerRet(const std::string& sMsgID, bool bRet) = 0;
    virtual void GetAllDealerRet(const std::string& sMsgID, const SetUInt64& setID) = 0;
    virtual void QueryPurchaseStatusRet(const std::string& sMsgID, const MapPurchaseStatus& mapPStatus) = 0; 
    virtual void ConfirmPurchaseRet(const std::string& sMsgID, int nStatus) = 0;
    virtual void IsEditPurchaseValidRet(const std::string& sMsgID, bool bRet) = 0;
    virtual void PurchaseStatusPush(const std::string& sMsgID, const MapPurchaseStatus& mapPStatus) = 0;
    virtual void QueryMMQuotStatusRet(const std::string& sMsgID, const LstQuotStatus& lstRet) = 0;
    virtual void QueryBondQuotByCodeStatusRet(const std::string& sMsgID, const LstQuotStatus& lstRet) = 0;
    virtual void ContactIsDealerPush(const std::string& sMsgID, const Contact& ct, bool bRet) = 0;
    virtual void IsEditPurchaseValidPush(const std::string& sMsgID, const Contact& ct, bool bRet) = 0;
    virtual void QueryQMIDByQBAccount(const std::string& sMsgID, const SetString& setQBAccount) = 0;
    virtual void QueryQMRemindConfig(const string& sMsgID) = 0;
    virtual void AddQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item) = 0;
    virtual void ModifyQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item) = 0;
    virtual void DelQMRemindKeyItem(const string& sMsgID, const SetString& setID) = 0;
    virtual void AddQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item) = 0;
    virtual void ModifyQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item) = 0;
    virtual void DelQMRemindFriendItem(const string& sMsgID, const SetString& setID) = 0;
    virtual void SetQMRemindType(const string& sMsgID, unsigned int nRemindType) = 0;
    virtual void OpenNewsBulletin(const std::string& sWndID) = 0;
    virtual bool QueryCustomService(const string& sMsgID) = 0;
    virtual void QBModifyPwd(const std::string& sNewPwd) = 0;
    virtual bool ShareBond(const LstContact& Contacts, const LstBondShare& lstBS, const std::string& sWndID) = 0;
    virtual bool ReqJoinGroup(const std::string& sMsgID, const unsigned long long &llGroup) = 0;
	virtual void ReqOpenNews(const NewsInfo &info) = 0;
    virtual void SharePic(const PicInfo &info, const LstContact& Contacts) = 0;
    virtual void ShareNewsDetail(const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF = true) = 0;
    virtual void OpenDialogueByUserID(const std::string &userID) = 0;
    virtual void QueryQMStatus() = 0;
    virtual void ReqOpenSellsWnd(const std::string& sWndID) = 0;
    virtual void ReqQueryNoInstituteContact(const std::string& sMsgID) = 0;
    virtual void ReqSetNoInstituteContact(const std::list<std::string> &QBAccounts) = 0;
    virtual void ReqOpenSellsSearchWnd(const std::string& sWndID = "") = 0;
    virtual void ReqOpenDialogueFromAd(const std::string &userID) = 0;
};