#pragma once
#include <string>
#include "QB_QMDefine.h"

using std::string;
using namespace QB_QM;

class IQBCommFrame
{
protected:
	IQBCommFrame(){}
public:
	virtual ~IQBCommFrame(){}

	virtual	void SetQMStatus(int nStatus, string sSessionID) = 0;
	virtual string GetSessionID() = 0;
	virtual bool SelContactsRet(const std::string& sSelID, const LstContact& Contacts) = 0;
	virtual bool QueryContactInfoRet(LstPersonInfo& PersonInfos, LstGroupInfo& GrpInfos, const LstContact& lstFailed) = 0;
	virtual bool EditContactsRet(const std::string& sEditID, const LstContact& Contacts) = 0;
	virtual bool SendQuotRet(LstQuotSendInfo& lstSuccess, LstQuotSendInfo& lstFailed) = 0;
	virtual bool RequestBondInfos() = 0;
	virtual bool ShowDlgBondDetail(const std::string& sBondKey) = 0;
	virtual bool RecvQuot(const LstQuotRecvInfo& lstRecv) = 0;
	virtual void QueryCurUsrIDRet(unsigned long long UsrID) = 0;
	virtual bool SendBondQuotByCodeRet(const LstBondQuotByCodeSendInfo& lstSuccess, const LstBondQuotByCodeSendInfo& lstFailed) = 0;
	virtual bool SendBondQuotByConditionRet(const LstBondQuotByConditionSendInfo& lstSuccess, const LstBondQuotByConditionSendInfo& lstFailed) = 0;
	virtual void QueryCurUsrFriendIDsRet(const SetUInt64& setIDs) = 0;
	virtual void QueryCurUsrGroupIDsRet(const SetUInt64& setIDs) = 0;
	virtual void CurUsrGroupChange(const SetUInt64& JoinGrpIDs, const SetUInt64& LeaveGrpIDs) = 0;
    virtual void CurUsrFriendChange(const SetUInt64& AddFriendIDs, const SetUInt64& DelFriendIDs) = 0;
	virtual void GroupDisband(unsigned long long GrpID) = 0;
    virtual bool QueryQBWndPos(const std::string& sMsgID, const std::string& sWndID) = 0;
    virtual bool QueryQBWndHandle(const std::string& sMsgID, const std::string& sWndID) = 0;
    virtual bool QueryQBPerm(const std::string& sMsgID) = 0;
    virtual void OpenMMQuot(const LstContact& Contacts, const std::string& sWndID) = 0;
    virtual void SearchQMContactRet(const std::string& sMsgID, const LstSearchUserInfo& Result) = 0;
    virtual void QueryContactIsDealer(const std::string& sMsgID, const Contact& ct) = 0;
    virtual void OpenSubscribeDlg(const std::string& sMsgID, const Contact& ct, const std::string& sWndID) = 0;
    virtual void GetAllDealer(const std::string& sMsgID) = 0;
    virtual bool SendMMQuotRetV2(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed) = 0;
    virtual bool SendBondQuotByCodeRetV2(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed) = 0;
    virtual void QMErrInfo(int nErrCode, const std::string& sErrInfo) = 0;
    virtual void QueryPurchaseStatus(const std::string& sMsgID, const LstPurchaseUkey& lstQuery) = 0;
    virtual void ConfirmPurchase(const std::string& sMsgID, const std::string& sPurchaseID, time_t ModifyTime) = 0; 
    virtual void IsEditPurchaseValid(const std::string& sMsgID, const Contact& ct) = 0;
    virtual void EditPurchase(const std::string& sMsgID, const Contact& ct, const std::string& sWndID) = 0;
    virtual void QueryMMQuotStatus(const std::string& sMsgID, const LstQuotStatusReq& lstReq) = 0;
    virtual void QueryBondQuotByCodeStatus(const std::string& sMsgID, const LstQuotStatusReq& lstReq) = 0;
    virtual void ContactInfoPush(const std::string& sMsgID, const LstPersonInfo& PersonInfos, const LstGroupInfo& GrpInfos) = 0;
    virtual void ContactStatusPush(const std::string& sMsgID, const LstContactStatus& lstStatus) = 0;
    virtual void QueryQMIDByQBAccountRet(const std::string& sMsgID, const LstQBAccount2QMID& lstRet) = 0;
    virtual void QueryQMRemindConfigRet(const std::string& sMsgID, const QMRemindConfig& QMRCfg) = 0;
    virtual void AddQMRemindKeyItemRet(const std::string& sMsgID, const std::string& ItemID) = 0;
    virtual void ModifyQMRemindKeyItemRet(const std::string& sMsgID, bool bSuccess) = 0;
    virtual void DelQMRemindKeyItemRet(const std::string& sMsgID, const SetString& setSuccessID) = 0;
    virtual void AddQMRemindFriendItemRet(const std::string& sMsgID, const std::string& ItemID) = 0;
    virtual void ModifyQMRemindFriendItemRet(const std::string& sMsgID, bool bSuccess) = 0;
    virtual void DelQMRemindFriendItemRet(const std::string& sMsgID, const SetString& setSuccessID) = 0;
    virtual void SetQMRemindTypeRet(const std::string& sMsgID, bool bSuccess) = 0;
    virtual void ReqJoinGroupRet(const std::string& sMsgID, bool bSuccess) = 0;
    virtual void ReqOpenNewsDetail(const NewsInfoV2 &info, bool bDisablePDF = true) = 0;
    virtual void ReqOpenNewsWnd() = 0;
    virtual void QueryQMStatusRet(int status) = 0;
    virtual void ModifyNoInstituteContactRet(const LstNoInstituteContact& Contacts) = 0;
};