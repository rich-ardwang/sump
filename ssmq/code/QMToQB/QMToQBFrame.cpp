#include "stdafx.h"
#include "QMToQBFrame.h"
#include "MsgMng.h"
#include <process.h>  
#include "QMObs_Mng.h"
#include "QMObs_QBStatus.h"
#include "QMObs_Session1.h"
#include "QMObs_Session2.h"
#include "QMObs_Session3.h"
#include "QMObs_Session4.h"
#include "QMObs_Session5.h"
#include "QMObs_SessionEx.h"
#include "SSRTL\SSLog.h"
#include "LogMarco.h"
#include "pubcode/Md5.h"
#include "MsgQueueMap.h"

#define UNIQUE_MUTEXNAME_ML "410A335B-3900-468b-99E6-732CE7D0CD63"

IQMToQB* GetQMToQB()
{
	return CQMToQBFrame::Instance();
}

CQMToQBFrame::CQMToQBFrame(void)
:m_sUsr("")
,m_sPwd("")
,m_bStopHeartbeat(false)
,m_HeartbeatThread(NULL)
,m_nState(0)
,m_pCallback(NULL)
,m_pCallback_MM(NULL)
,m_pCallback_Bond(NULL)
,m_pCallback_Remind(NULL)
,m_pLogger(NULL)
,m_hMultiLoginMutex(NULL)
,m_bStopGuardThread(false)
,m_GuardThread(NULL)
,m_nQBStatus(0)
{
	m_pLogger = new ss::logger("log\\QMToQB");
	m_pLogger->set_level_by_config("config\\logconfig.ini", "maincfg.qmtoqb");
}

CQMToQBFrame::~CQMToQBFrame(void)
{
	StopHeartbeat();
	StopGuardThread();
	ClearObs();
	if (m_pLogger)
	{
		delete m_pLogger;
		m_pLogger = NULL;
	}

    if (m_hMultiLoginMutex)
    {
        CloseHandle(m_hMultiLoginMutex);
        m_hMultiLoginMutex = NULL;
    }
}

void CQMToQBFrame::Heartbeat()
{
	sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(1, "heartbeat");
	values.SetInt32(2, m_nState);
	values.SetString(3, GetSessionID());

	GetSSMQMng()->Publish("qm.status", msg);
}

void CQMToQBFrame::CloseClient()
{
	if (!m_pCallback)
	{
		return;
	}
	StopHeartbeat();
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::CloseClient\n");
	m_pCallback->QuitQM();
}

void CQMToQBFrame::Init(const std::string& sUsr, const std::string& sPwd)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "Init QMToQB by User: %s\n", sUsr.c_str());

	GetSSMQMng()->InitServer();
	m_sUsr = sUsr;
	m_sPwd = sPwd;
	InitObs();
	StartHeartbeat();
	StartGuardThread();
	GetSSMQMng()->SetMngCallback(this);
}

void CQMToQBFrame::Release()
{
	StopHeartbeat();
	GetSSMQMng()->ReleaseServer();
}

void CQMToQBFrame::RegCallback(IQMToQBCallback* pCallback)
{
	m_pCallback = pCallback;
    if (m_pCallback)
    {
		CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "QBStatus Change! Status = %d\n", m_nQBStatus);
        m_pCallback->QBStatus(m_nQBStatus);
    }
}

void CQMToQBFrame::CloseLocalDataBus()
{
    GetSSMQMng()->CloseLocalDataBus();
}

IQMToQB_MM* CQMToQBFrame::GetSubInterface_MM()
{
	return this;
}

IQMToQB_Bond* CQMToQBFrame::GetSubInterface_Bond()
{
	return this;
}

IQMToQB_Purchase* CQMToQBFrame::GetSubInterface_Purchase()
{
    return this;
}

IQMToQB_Remind* CQMToQBFrame::GetSubInterface_Remind()
{
    return this;
}

bool CQMToQBFrame::SelContactsRet(const std::string& sSelID, const LstContact& Contacts)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SelContactsRet, sSelID = %s\n", sSelID.c_str());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SelContactsRet(sSelID, Contacts, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQMToQBFrame::EditContactsRet(const std::string& sEditID, const LstContact& Contacts)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::EditContactsRet, sEditID = %s\n", sEditID.c_str());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_EditContactsRet(sEditID, Contacts, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

void CQMToQBFrame::SetStatus(int nStatus)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SetStatus, nStatus = %d\n", nStatus);
	m_nState = nStatus;
	Heartbeat();
}

//bool CQMToQBFrame::RecvMsgs(const LstMsgInfo& lstMsg)
//{
//	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::RecvMsgs, lstMsg Size = %d\n", lstMsg.size());
//	return true;
//}

//bool CQMToQBFrame::ReadMsgsOfContact(const Contact& ct)
//{
//	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ReadMsgsOfContact\n");
//	return true;
//}

//bool CQMToQBFrame::RecvQuot(const LstQuotRecvInfo& lstRecv)
//{
//	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::RecvQuot, lstRecv Size = %d\n", lstRecv.size());
//
//	sdbus::Message msg;
//	CMsgMng::Instance()->GetMsg_RecvQuot(lstRecv, msg);
//	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
//	return true;
//}

void CQMToQBFrame::QueryCurUsrFriendIDsRet(const SetUInt64& setIDs)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryCurUsrFriendIDsRet, setIDs Size = %d\n", setIDs.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QueryCurUsrFriendIDsRet(setIDs, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::QueryCurUsrGroupIDsRet(const SetUInt64& setIDs)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryCurUsrGroupIDsRet, setIDs Size = %d\n", setIDs.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QueryCurUsrGroupIDsRet(setIDs, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::CurUsrGroupChange(const SetUInt64& JoinGrpIDs, const SetUInt64& LeaveGrpIDs)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::CurUsrGroupChange, JoinGrpIDs Size = %d, LeaveGrpIDs Size = %d\n", JoinGrpIDs.size(), LeaveGrpIDs.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_CurUsrGroupChange(JoinGrpIDs, LeaveGrpIDs, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::CurUsrFriendChange(const SetUInt64& AddFriendIDs, const SetUInt64& DelFriendIDs)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::CurUsrFriendChange, AddFriendIDs Size = %d, DelFriendIDs Size = %d\n", AddFriendIDs.size(), DelFriendIDs.size());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_CurUsrFriendChange(AddFriendIDs, DelFriendIDs, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::GroupDisband(unsigned long long GrpID)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::GroupDisband\n");

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_GroupDisband(GrpID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

bool CQMToQBFrame::QueryQBWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight, int timeout/* = -1*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryQBWndPos\n");

    sdbus::Message msgReq;
    CMsgMng::Instance()->GetMsg_QueryQBWndPos(sWndID, msgReq);
    msgReq.SetMessageID(GetSSMQMng()->GetUniqueMsgID());

    sdbus::Message msgAck;
    bool bRet = GetSSMQMng()->Send(GetSendQueueID(msgReq), msgReq, msgAck, timeout);
    CMsgMng::Instance()->ParseMsg_QueryQBWndPosRet(msgAck, nTop, nBottom, nLeft, nRight);
    return bRet;
}

bool CQMToQBFrame::QueryQBWndHandle(const std::string& sWndID, unsigned long long& hWnd, int timeout/* = -1*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryQBWndHandle\n");

    sdbus::Message msgReq;
    CMsgMng::Instance()->GetMsg_QueryQBWndHandle(sWndID, msgReq);
    msgReq.SetMessageID(GetSSMQMng()->GetUniqueMsgID());

    sdbus::Message msgAck;
    bool bRet = GetSSMQMng()->Send(GetSendQueueID(msgReq), msgReq, msgAck, timeout);
    CMsgMng::Instance()->ParseMsg_QueryQBWndHandleRet(msgAck, hWnd);
    return bRet;
}

bool CQMToQBFrame::QueryQBPerm(SetString& setPerm, int timeout/* = -1*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryQBPerm\n");

    sdbus::Message msgReq;
    CMsgMng::Instance()->GetMsg_QueryQBPerm(msgReq);
    msgReq.SetMessageID(GetSSMQMng()->GetUniqueMsgID());

    sdbus::Message msgAck;
    bool bRet = GetSSMQMng()->Send(GetSendQueueID(msgReq), msgReq, msgAck, timeout);
    CMsgMng::Instance()->ParseMsg_QueryQBPermRet(msgAck, setPerm);
    return bRet;
}

void CQMToQBFrame::OpenMMQuot(const LstContact& Contacts, const std::string& sWndID/* = ""*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::OpenMMQuot\n");

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_OpenMMQuot(Contacts, sWndID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

int CQMToQBFrame::GetMultiLoginStatus()
{
    m_hMultiLoginMutex = CreateMutexA(NULL, FALSE, UNIQUE_MUTEXNAME_ML);
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 1;
    }
    return 0;
}

void CQMToQBFrame::ResetMultiLoginStatus()
{
    if (m_hMultiLoginMutex)
    {
        CloseHandle(m_hMultiLoginMutex);
        m_hMultiLoginMutex = NULL;
    }
}

void CQMToQBFrame::QMErrInfo(int nErrCode, const std::string& sErrInfo)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QMErrInfo\n");
    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QMErrInfo(nErrCode, sErrInfo, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ContactInfoPush(const LstPersonInfo& PersonInfos, const LstGroupInfo& GrpInfos)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ContactInfoPush, PersonInfos Cnt = %d, GrpInfos Cnt = %d\n", PersonInfos.size(), GrpInfos.size());
    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ContactInfoPush(PersonInfos, GrpInfos, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ContactStatusPush(const LstContactStatus& lstStatus)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ContactStatusPush, lstStatus Cnt = %d\n", lstStatus.size());
    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ContactStatusPush(lstStatus, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::QueryQMIDByQBAccountRet(const std::string& sMsgID, const LstQBAccount2QMID& lstRet)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryQMIDByQBAccountRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQMIDByQBAccountRet(lstRet, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::RegCallback(IQMToQBCallback_MM* pCallback)
{
	m_pCallback_MM = pCallback;
}

bool CQMToQBFrame::SendQuotRet(LstQuotSendInfo& lstSuccess, LstQuotSendInfo& lstFailed)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SendQuotRet, Success Num = %d, Failed Num = %d\n", lstSuccess.size(), lstFailed.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SendQuotRet(lstSuccess, lstFailed, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQMToQBFrame::SendQuotRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SendQuotRetV2, Success Num = %d, Failed Num = %d\n", Success.size(), Failed.size());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_SendQuotRetV2(ct, Success, Failed, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return true;
}

std::string CQMToQBFrame::QueryMMQuotStatus(const LstQuotStatusReq& lstReq)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryMMQuotStatus, MsgID = %s, Cnt = %d\n", sMsgID.c_str(), lstReq.size());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryMMQuotStatus(lstReq, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

void CQMToQBFrame::RegCallback(IQMToQBCallback_Bond* pCallback)
{
	m_pCallback_Bond = pCallback;
}

bool CQMToQBFrame::RequestBondInfos()
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::RequestBondInfos\n");

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_RequestBondInfos(msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQMToQBFrame::ShowDlgBondDetail(const std::string& sBondKey)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ShowDlgBondDetail, sBondKey = %s\n", sBondKey.c_str());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_ShowDlgBondDetail(sBondKey, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQMToQBFrame::SendBondQuotByCodeRet(const LstBondQuotByCodeSendInfo& lstSuccess, const LstBondQuotByCodeSendInfo& lstFailed)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SendBondQuotByCodeRet, Success Num = %d, Failed Num = %d\n", lstSuccess.size(), lstFailed.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SendBondQuotByCodeRet(lstSuccess, lstFailed, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQMToQBFrame::SendBondQuotByCodeRet(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SendBondQuotByCodeRetV2, Success Num = %d, Failed Num = %d\n", Success.size(), Failed.size());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_SendBondQuotByCodeRetV2(ct, Success, Failed, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return true;
}

bool CQMToQBFrame::SendBondQuotByConditionRet(const LstBondQuotByConditionSendInfo& lstSuccess, const LstBondQuotByConditionSendInfo& lstFailed)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SendBondQuotByConditionRet, Success Num = %d, Failed Num = %d\n", lstSuccess.size(), lstFailed.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SendBondQuotByConditionRet(lstSuccess, lstFailed, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

std::string CQMToQBFrame::QueryBondQuotByCodeStatus(const LstQuotStatusReq& lstReq)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryBondQuotByCodeStatus, MsgID = %s, Cnt = %d\n", sMsgID.c_str(), lstReq.size());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryBondQuotByCodeStatus(lstReq, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

void CQMToQBFrame::RegCallback(IQMToQBCallback_Purchase* pCallback)
{
    m_pCallback_Purchase = pCallback;
}

std::string CQMToQBFrame::ContactIsDealer(const Contact& ct)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ContactIsDealer, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ContactIsDealer(ct, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQMToQBFrame::GetAllDealer()
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::GetAllDealer, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_GetAllDealer(msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

void CQMToQBFrame::OpenSubscribeDlg(const Contact& ct, const std::string& sWndID/* = ""*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::OpenSubscribeDlg\n");

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_OpenSubscribeDlg(ct, sWndID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

std::string CQMToQBFrame::QueryPurchaseStatus(const LstPurchaseUkey& lstQuery)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryPurchaseStatus, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryPurchaseStatus(lstQuery, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQMToQBFrame::ConfirmPurchase(const std::string& sPurchaseID, time_t ModifyTime)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ConfirmPurchase, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_ConfirmPurchase(sPurchaseID, ModifyTime, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQMToQBFrame::IsEditPurchaseValid(const Contact& ct)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::IsEditPurchaseValid, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_IsEditPurchaseValid(ct, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

void CQMToQBFrame::EditPurchase(const Contact& ct, const std::string& sWndID/* = ""*/)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::EditPurchase, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_EditPurchase(ct, sWndID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::RegCallback(IQMToQBCallback_Remind* pCallback)
{
    m_pCallback_Remind = pCallback;
}

void CQMToQBFrame::QueryQMRemindConfigRet(const string& sMsgID, const QMRemindConfig& QMRCfg)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryQMRemindConfigRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->BuildQueryQMRemindConfigRet(QMRCfg, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::AddQMRemindKeyItemRet(const string& sMsgID, const string& ItemID)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::AddQMRemindKeyItemRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_AddQMRemindKeyItemRet(ItemID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ModifyQMRemindKeyItemRet(const string& sMsgID, bool bSuccess)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ModifyQMRemindKeyItemRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_ModifyQMRemindKeyItemRet(bSuccess, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::DelQMRemindKeyItemRet(const string& sMsgID, const SetString& setSuccessID)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::DelQMRemindKeyItemRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_DelQMRemindKeyItemRet(setSuccessID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::AddQMRemindFriendItemRet(const string& sMsgID, const string& ItemID)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::AddQMRemindFriendItemRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_AddQMRemindFriendItemRet(ItemID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ModifyQMRemindFriendItemRet(const string& sMsgID, bool bSuccess)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ModifyQMRemindFriendItemRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_ModifyQMRemindFriendItemRet(bSuccess, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::DelQMRemindFriendItemRet(const string& sMsgID, const SetString& setSuccessID)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::DelQMRemindFriendItemRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_DelQMRemindFriendItemRet(setSuccessID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::SetQMRemindTypeRet(const string& sMsgID, bool bSuccess)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::SetQMRemindTypeRet, MsgID = %s\n", sMsgID.c_str());
    sdbus::Message msg;
    msg.SetMessageID(sMsgID);
    CMsgMng::Instance()->GetMsg_SetQMRemindTypeRet(bSuccess, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::Quit(bool bChkSession, const std::string& sSessionID)
{
	if (bChkSession && sSessionID != GetSessionID())
	{
		return;
	}

	if (!m_pCallback)
	{
		return;
	}
	StopHeartbeat();
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::Quit\n");
	m_pCallback->QuitQM();
}

string CQMToQBFrame::GetSessionID()
{
	string sRet = m_sUsr;
	sRet += "$$";
	sRet += m_sPwd;

	MD5 EncodeTool;
	EncodeTool.update(sRet);
	return EncodeTool.toString();
}

void CQMToQBFrame::SetQBStatus(int nStatus, string sSessionID)
{
	if (sSessionID != GetSessionID())
	{
		return;
	}

	if (m_bStopGuardThread)
	{
		StopGuardThread();
		StartGuardThread();
	}

	m_conditionHB.notify_one();

	if (m_nQBStatus == nStatus)
	{
		return;
	}

	m_nQBStatus = nStatus;
    if (m_pCallback)
    {
		CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "QBStatus Change! Status = %d\n", m_nQBStatus);
        m_pCallback->QBStatus(m_nQBStatus);
    }
}

void CQMToQBFrame::SelContacts(const std::string& sSelID, const std::string& sWndID, int nLimit, bool bShowGrp)
{
	if (!m_pCallback)
	{
		return;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SelContacts, sSelID = %s\n", sSelID.c_str());
	m_pCallback->SelContacts(sSelID, sWndID, nLimit, bShowGrp);
}

void CQMToQBFrame::EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID, int nLimit, bool bShowGrp)
{
	if (!m_pCallback)
	{
		return;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::EditContacts, sEditID = %s\n", sEditID.c_str());
	m_pCallback->EditContacts(sEditID, Contacts, sWndID, nLimit, bShowGrp);
}

void CQMToQBFrame::SendQuot(const LstQuotSendInfo& lstSend)
{
	if (!m_pCallback)
	{
		return;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SendQuotV1, content size %d\n", lstSend.size());
	//m_pCallback_MM->SendQuot(lstSend);
}

void CQMToQBFrame::QueryContactInfo(const std::string& sMsgID, const LstContact& lstIn)
{
	LstPersonInfo PersonInfos;
	LstGroupInfo GrpInfos;
    LstContact lstFailed;
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryContactInfo, content size %d\n", lstIn.size());
	if (m_pCallback->QueryContactInfo(lstIn, PersonInfos, GrpInfos, lstFailed))
	{
		sdbus::Message msg;
		CMsgMng::Instance()->GetMsg_QueryContactsInfoRet(PersonInfos, GrpInfos, lstFailed, msg);
        msg.SetMessageID(sMsgID);
		CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryContactInfoRet\n");
		GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	}
}

void CQMToQBFrame::RequestBondInfosRet(const LstBondItem& LstRet)
{
	if (!m_pCallback_Bond)
	{
		return;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::RequestBondInfosRet, content size %d\n", LstRet.size());
	m_pCallback_Bond->RequestBondInfosRet(LstRet);
}

void CQMToQBFrame::OpenMainPanel(const std::string& sWndID)
{
	if (!m_pCallback)
	{
		return;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::OpenMainPanel\n");
	m_pCallback->OpenMainPanel(sWndID);
}

bool CQMToQBFrame::RefreshBondInfos()
{
	//if (!m_pCallback)
	//{
	//	return false;
	//}

	//CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::RefreshBondInfos\n");
	//return m_pCallback->RefreshBondInfos();
	return false;
}

bool CQMToQBFrame::OpenDialogue(const Contact& ct, const std::string& sSendMsg, const std::string& sWndID, const bool bOpen)
{
	if (!m_pCallback)
	{
		return false;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::OpenDialogue\n");
	return m_pCallback->OpenDialogue(ct, sSendMsg, sWndID, bOpen);
}

bool CQMToQBFrame::SendBondQuotByCode(const LstBondQuotByCodeSendInfo& lstSend)
{
	if (!m_pCallback_Bond)
	{
		return false;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SendBondQuotByCodeV1, lstSend Size = %d\n", lstSend.size());
	//return m_pCallback_Bond->SendBondQuotByCode(lstSend);
    return false;
}

bool CQMToQBFrame::SendBondQuotByCondition(const LstBondQuotByConditionSendInfo& lstSend)
{
	if (!m_pCallback_Bond)
	{
		return false;
	}

	//CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SendBondQuotByCondition, lstSend Size = %d\n", lstSend.size());
	//return m_pCallback_Bond->SendBondQuotByCondition(lstSend);
    return true;
}

bool CQMToQBFrame::ReplyQuot(const QuotReplyInfo& Reply, const std::string& sWndID)
{
	if (!m_pCallback_MM)
	{
		return false;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReplyQuot\n");
	return m_pCallback_MM->ReplyQuot(Reply, sWndID);

}

void CQMToQBFrame::QueryCurUsrID()
{
	if (!m_pCallback)
	{
		return;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryCurUsrID\n");

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QueryCurUsrIDRet(m_pCallback->GetCurUsrID(), msg);
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryCurUsrIDRet\n");
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

bool CQMToQBFrame::QueryCurUsrFriendIDs()
{
	if (!m_pCallback)
	{
		return false;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryCurUsrFriendIDs\n");
	return m_pCallback->QueryCurUsrFriendIDs();
}

bool CQMToQBFrame::QueryCurUsrGroupIDs()
{
	if (!m_pCallback)
	{
		return false;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryCurUsrGroupIDs\n");
	return m_pCallback->QueryCurUsrGroupIDs();
}

bool CQMToQBFrame::SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SendMsg\n");
    return m_pCallback->SendMsg(Contacts, sMsg, sWndID);
}

bool CQMToQBFrame::QueryQMWndPos(const std::string& sMsgID, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryQMWndPos\n");

    int nTop(0), nBottom(0), nLeft(0), nRight(0);
    bool bRet = m_pCallback->QueryQMWndPos(sWndID, nTop, nBottom, nLeft, nRight);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQMWndPosRet(nTop, nBottom, nLeft, nRight, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

bool CQMToQBFrame::QueryQMWndHandle(const std::string& sMsgID, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryQMWndHandle\n");
    
    unsigned long long hWnd(0);
    bool bRet = m_pCallback->QueryQMWndHandle(sWndID, hWnd);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQMWndHandleRet(hWnd, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

bool CQMToQBFrame::QuerySharedGroup(const std::string& sMsgID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QuerySharedGroup, MsgID = %s\n", sMsgID.c_str());
    Contact ct;
    bool bRet = m_pCallback->QuerySharedGroup(ct);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryShareGroupRet(ct, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

bool CQMToQBFrame::SearchQMContact(const std::string& sMsgID, const ContactSearch& Input)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SearchQMContact, MsgID = %s\n", sMsgID.c_str());
    LstSearchUserInfo SearchRet;
    bool bRet = m_pCallback->SearchQMContact(Input, SearchRet);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_SearchQMContactRet(SearchRet, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

bool CQMToQBFrame::ReplyBondQuotByCode(const BondQuotByCodeReplyInfo& Reply, const std::string& sWndID)
{
	if (!m_pCallback_Bond)
	{
		return false;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReplyBondQuotByCode\n");
	return m_pCallback_Bond->ReplyBondQuotByCode(Reply, sWndID);
}

void CQMToQBFrame::SendQuotV2(const std::string& sMsgID, const LstMMQuotSendInfoV2& lstSend)
{
    if (!m_pCallback)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SendQuotV2, content size %d\n", lstSend.size());
    m_pCallback_MM->SendQuot(sMsgID, lstSend);
}

void CQMToQBFrame::SendBondQuotByCodeV2(const std::string& sMsgID, const LstBondQuotByCodeSendInfoV2& lstSend)
{
    if (!m_pCallback_Bond)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SendBondQuotByCodeV2, lstSend Size = %d\n", lstSend.size());
    m_pCallback_Bond->SendBondQuotByCode(sMsgID, lstSend);
}

void CQMToQBFrame::Purchase(const std::string& sMsgID, const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::Purchase, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->Purchase(ct, PInfo, sWndID);
}

void CQMToQBFrame::ContactIsDealerRet(const std::string& sMsgID, bool bRet)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ContactIsDealerRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->ContactIsDealerRet(sMsgID, bRet);
}

void CQMToQBFrame::GetAllDealerRet(const std::string& sMsgID, const SetUInt64& setID)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::GetAllDealerRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->GetAllDealerRet(sMsgID, setID);
}

void CQMToQBFrame::QueryPurchaseStatusRet(const std::string& sMsgID, const MapPurchaseStatus& mapPStatus)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryPurchaseStatusRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->QueryPurchaseStatusRet(sMsgID, mapPStatus);
}

void CQMToQBFrame::ConfirmPurchaseRet(const std::string& sMsgID, int nStatus)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ConfirmPurchaseRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->ConfirmPurchaseRet(sMsgID, nStatus);
}

void CQMToQBFrame::IsEditPurchaseValidRet(const std::string& sMsgID, bool bRet)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::IsEditPurchaseValidRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->IsEditPurchaseValidRet(sMsgID, bRet);
}

void CQMToQBFrame::PurchaseStatusPush(const std::string& sMsgID, const MapPurchaseStatus& mapPStatus)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::PurchaseStatusPush, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->PurchaseStatusPush(mapPStatus);
}

void CQMToQBFrame::QueryMMQuotStatusRet(const std::string& sMsgID, const LstQuotStatus& lstRet)
{
    if (!m_pCallback_MM)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryMMQuotStatusRet, MsgID = %s, Cnt = %d\n", sMsgID.c_str(), lstRet.size());
    m_pCallback_MM->QueryMMQuotStatusRet(sMsgID, lstRet);
}

void CQMToQBFrame::QueryBondQuotByCodeStatusRet(const std::string& sMsgID, const LstQuotStatus& lstRet)
{
    if (!m_pCallback_Bond)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryBondQuotByCodeStatus, MsgID = %s, Cnt = %d\n", sMsgID.c_str(), lstRet.size());
    m_pCallback_Bond->QueryBondQuotByCodeStatusRet(sMsgID, lstRet);
}

void CQMToQBFrame::ContactIsDealerPush(const std::string& sMsgID, const Contact& ct, bool bRet)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ContactIsDealerPush, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->ContactIsDealerPush(ct, bRet);
}

void CQMToQBFrame::IsEditPurchaseValidPush(const std::string& sMsgID, const Contact& ct, bool bRet)
{
    if (!m_pCallback_Purchase)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::IsEditPurchaseValidPush, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Purchase->IsEditPurchaseValidPush(ct, bRet);
}

void CQMToQBFrame::QueryQMIDByQBAccount(const std::string& sMsgID, const SetString& setQBAccount)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryQMIDByQBAccount, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->QueryQMIDByQBAccount(sMsgID, setQBAccount);
}

void CQMToQBFrame::QueryQMRemindConfig(const string& sMsgID)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryQMRemindConfig, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->QueryQMRemindConfig(sMsgID);
}

void CQMToQBFrame::AddQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::AddQMRemindKeyItem, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->AddQMRemindKeyItem(sMsgID, Item);
}

void CQMToQBFrame::ModifyQMRemindKeyItem(const string& sMsgID, const QMRemindKeyItem& Item)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ModifyQMRemindKeyItem, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->ModifyQMRemindKeyItem(sMsgID, Item);
}

void CQMToQBFrame::DelQMRemindKeyItem(const string& sMsgID, const SetString& setID)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::DelQMRemindKeyItem, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->DelQMRemindKeyItem(sMsgID, setID);
}

void CQMToQBFrame::AddQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::AddQMRemindFriendItem, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->AddQMRemindFriendItem(sMsgID, Item);
}

void CQMToQBFrame::ModifyQMRemindFriendItem(const string& sMsgID, const QMRemindFriendItem& Item)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ModifyQMRemindFriendItem, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->ModifyQMRemindFriendItem(sMsgID, Item);
}

void CQMToQBFrame::DelQMRemindFriendItem(const string& sMsgID, const SetString& setID)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::DelQMRemindFriendItem, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->DelQMRemindFriendItem(sMsgID, setID);
}

void CQMToQBFrame::SetQMRemindType(const string& sMsgID, unsigned int nRemindType)
{
    if (!m_pCallback_Remind)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SetQMRemindType, MsgID = %s\n", sMsgID.c_str());
    m_pCallback_Remind->SetQMRemindType(sMsgID, nRemindType);
}

void CQMToQBFrame::OpenNewsBulletin(const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::OpenNewsBulletin\n");
    m_pCallback->OpenNewsBulletin(sWndID);
}

bool CQMToQBFrame::QueryCustomService(const string& sMsgID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryCustomService, MsgID = %s\n", sMsgID.c_str());
    Contact ct;
    bool bRet = m_pCallback->QueryCustomService(ct);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryCustomServiceRet(ct, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

void CQMToQBFrame::QBModifyPwd(const std::string& sNewPwd)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::CQMToQBFrame\n");
    m_pCallback->QBModifyPwd(sNewPwd);
}

bool CQMToQBFrame::ShareBond(const LstContact& Contacts, const LstBondShare& lstBS, const std::string& sWndID)
{
    if (!m_pCallback_Bond)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ShareBond\n");
    return m_pCallback_Bond->ShareBond(Contacts, lstBS, sWndID);
}

unsigned int __stdcall HeartbeatThread(PVOID pM)
{
	unsigned int nRet = 0;
	CQMToQBFrame *pThis = (CQMToQBFrame*)pM;


	while (!pThis->m_bStopHeartbeat)
	{
		pThis->Heartbeat();
		int n = 5;
		while (n > 1)
		{
			if (pThis->m_bStopHeartbeat)
			{
				break;
			}
			else
			{
				Sleep(200);
			}
			n--;
		}
	}

	return nRet;
}

void CQMToQBFrame::StartHeartbeat()
{
	m_bStopHeartbeat = false;
	m_HeartbeatThread = (HANDLE)_beginthreadex(NULL, 0, HeartbeatThread, this, 0, NULL);
}

void CQMToQBFrame::StopHeartbeat()
{
	if (NULL == m_HeartbeatThread)
	{
		return;
	}
	m_bStopHeartbeat = true;
	WaitForSingleObject(m_HeartbeatThread, 510);
	CloseHandle(m_HeartbeatThread);
	m_HeartbeatThread = NULL;
}

void CQMToQBFrame::InitObs()
{
	ClearObs();

	m_LstObs.push_back(new CQMObs_Mng());
	m_LstObs.push_back(new CQMObs_QBStatus());
	//m_LstObs.push_back(new CQMObs_Session());
	m_LstObs.push_back(new CQMObs_Session1());
	m_LstObs.push_back(new CQMObs_Session2());
	m_LstObs.push_back(new CQMObs_Session3());
	m_LstObs.push_back(new CQMObs_Session4());
    m_LstObs.push_back(new CQMObs_Session5());
    m_LstObs.push_back(new CQMObs_Session6());
    m_LstObs.push_back(new CQMObs_Session7());

	for (LstObs::iterator iter = m_LstObs.begin(); iter != m_LstObs.end(); ++iter)
	{
		(*iter)->SetCommFrame(this);
		GetSSMQMng()->RegObs(*iter);
	}
}

void CQMToQBFrame::ClearObs()
{
    for (LstObs::iterator iter = m_LstObs.begin(); iter != m_LstObs.end(); ++iter)
    {
        GetSSMQMng()->ReleaseObs(*iter);
    }
	ss_tool::ClearContainer(m_LstObs);
}

string CQMToQBFrame::GetSendQueueID(const sdbus::Message& msgs)
{
	static CMsgQueueMap mapTool;
	string sRet = mapTool.GetQueueHead(msgs);
	sRet += ".";
	sRet += GetSessionID();
	return sRet;
}


void CQMToQBFrame::GuardThread()
{
	while (!m_bStopGuardThread)
	{
		if (!WaitforHeartbeat())
		{
			int nLimitCnt = 0;//ÐÄÌøÈÝ´í´ÎÊý
			bool bAlive = false;
			while (nLimitCnt < 3 && !m_bStopGuardThread)
			{
				nLimitCnt++;
				if (WaitforHeartbeat())
				{
					bAlive = true;
					break;
				}
			}
			if (bAlive)
			{
				continue;
			}
			HBTimeout();
			m_bStopGuardThread = true;
		}
	}

}

void CQMToQBFrame::StartGuardThread()
{
	if (m_GuardThread)
	{
		return;
	}
	m_bStopGuardThread = false;
	m_GuardThread = new boost::thread(boost::bind(&CQMToQBFrame::GuardThread, this));
}

void CQMToQBFrame::StopGuardThread()
{
	if (m_GuardThread)
	{
		m_bStopGuardThread = true;
		m_GuardThread->join();
		delete m_GuardThread;
		m_GuardThread = NULL;
	}
}

bool CQMToQBFrame::WaitforHeartbeat()
{
	boost::mutex::scoped_lock lock(m_mutex) ;

	if (m_conditionHB.timed_wait(lock , boost::get_system_time() + boost::posix_time::millisec(1600)))
	{
		return true;
	}
	return false;
}

void CQMToQBFrame::HBTimeout()
{
	m_nQBStatus = 0;
	if (m_pCallback)
	{
		CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "QBStatus Change! Status = %d\n", m_nQBStatus);
		m_pCallback->QBStatus(m_nQBStatus);
	}
}

bool CQMToQBFrame::ReqJoinGroup( const std::string& sMsgID, const unsigned long long &llGroup )
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReqJoinGroup\n");
    m_pCallback->ReqJoinGroup(sMsgID, llGroup);

    return true;
}

void CQMToQBFrame::ReqJoinGroupRet( const std::string& sMsgID, bool bSuccess )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ReqJoinGroupRet");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildJoinGroupRet(bSuccess, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ReqOpenNews( const NewsInfo &info )
{
	if (!m_pCallback)
	{
		return;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReqOpenNews\n");
	m_pCallback->ReqOpenNews(info);
}

void CQMToQBFrame::SharePic( const PicInfo &info, const LstContact& Contacts )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::SharePic\n");
    m_pCallback->SharePic(info, Contacts);
}

void CQMToQBFrame::ReqOpenNewsDetail( const NewsInfoV2 &info, bool bDisablePDF /*= true*/ )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ReqOpenNewsDetail");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqOpenNewsDetail(info, bDisablePDF, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ReqOpenNewsWnd()
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ReqOpenNewsWnd");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqOpenNewsWnd(msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ShareNewsDetail( const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF /*= true*/ )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ShareNewsDetail\n");
    m_pCallback->ShareNewsDetail(info, Contacts, bDisablePDF);
}

void CQMToQBFrame::OpenDialogueByUserID( const std::string &userID )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::OpenDialogueByUserID\n");
    m_pCallback->OpenDialogueByUserID(userID);
}

void CQMToQBFrame::QueryQMStatus()
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::QueryQMStatus\n");
    m_pCallback->QueryQMStatus();
}

void CQMToQBFrame::QueryQMStatusRet( int status )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::QueryQMStatusRet status=%d\n", status);

    sdbus::Message msg;
    CMsgMng::Instance()->BuildQueryQMStatusRet(status, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ReqOpenSellsWnd( const std::string& sWndID )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReqOpenSellsWnd\n");
    m_pCallback->ReqOpenSellsWnd(sWndID);
}

void CQMToQBFrame::ModifyNoInstituteContactRet( const LstNoInstituteContact& contacts )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQMToQBFrame::ReqQueryNoInstituteContactRet size = %d\n", contacts.size());

    sdbus::Message msg;
    CMsgMng::Instance()->BuildModifyNoInstituteContactRet(contacts, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ReqQueryNoInstituteContact( const std::string& sMsgID )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReqQueryNoInstituteContact\n");

    LstNoInstituteContact contacts;
    int status(0);
    m_pCallback->ReqQueryNoInstituteContact(contacts, status);

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqQueryNoInstituteContactRet(contacts, status, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQMToQBFrame::ReqSetNoInstituteContact( const std::list<std::string> &QBAccounts )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReqSetNoInstituteContact\n");
    m_pCallback->ReqSetNoInstituteContact(QBAccounts);
}

void CQMToQBFrame::ReqOpenSellsSearchWnd( const std::string& sWndID )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReqOpenSellsSearchWnd\n");
    m_pCallback->ReqOpenSellsSearchWnd(sWndID);
}

void CQMToQBFrame::ReqOpenDialogueFromAd( const std::string &userID )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQMToQBFrame::ReqOpenDialogueFromAd\n");
    m_pCallback->ReqOpenDialogueFromAd(userID);
}
