#include "StdAfx.h"
#include "QBToQMFrame.h"
#include "QBObs_QMStatus.h"
#include "QBObs_Session1.h"
#include "QBObs_Session2.h"
#include "QBObs_Session3.h"
#include "QBObs_Session4.h"
#include "QBObs_Session5.h"
#include "QBObs_Session6.h"
#include "QBObs_SessionEx.h"
#include <Psapi.h>
#include <shellapi.h>
#include <process.h>  
#include "MsgMng.h"
#include "QBToQMObjCoder.h"
#include "SSRTL\SSLog.h"
#include "LogMarco.h"
#include <io.h>
#include "pubcode/Md5.h"
#include "MsgQueueMap.h"

#define UNIQUE_EVENTNAME_HB "652FC3DF-A442-4e70-BBFC-9E0E5B12CC76"
#define UNIQUE_MUTEXNAME_ML "410A335B-3900-468b-99E6-732CE7D0CD63"


IQBToQM* GetQBToQM()
{
	return CQBToQMFrame::Instance();
}

CQBToQMFrame::CQBToQMFrame(void)
:m_pCallback(NULL)
,m_nQMStatus(0)
,m_hHeartbeatEvent(NULL)
,m_hGuardThread(NULL)
,m_pLogger(NULL)
,m_hMultiLoginMutex(NULL)
,m_ThreadHeartbeat(NULL)
,m_bStopGuard(false)
,m_nState(0)
{
	m_pLogger = new ss::logger("log\\QBToQM");
	m_pLogger->set_level_by_config("config\\logconfig.ini", "maincfg.qbtoqm");
}

CQBToQMFrame::~CQBToQMFrame(void)
{
	ClearObs();
	StopGuardThread();
	StopHeartbeat();

	if (m_hHeartbeatEvent)
	{
		CloseHandle(m_hHeartbeatEvent);
		m_hHeartbeatEvent = NULL;
	}

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

void CQBToQMFrame::CloseClient()
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::CloseClient\n");
	if (m_pCallback)
	{
		m_pCallback->QuitQB();
	}
}

bool CQBToQMFrame::Init(const QBToQMInitParam& InitParam)
{
	m_sUsr = InitParam.sUsr;
	m_sPwd = InitParam.sPwd;
    m_stQMProxyInfo = InitParam.stQMProxyInfo;
	m_sQMDir = SS_BASE::char2t_string(InitParam.sQMDir.c_str());
	m_sQMExeName = SS_BASE::char2t_string(InitParam.sQMExeName.c_str());

	InitObs();

    string sHBUID = UNIQUE_EVENTNAME_HB + GetSessionID();
    m_hHeartbeatEvent = CreateEventA(NULL, FALSE, FALSE, sHBUID.c_str());

	std::string sChkPath = m_sQMDir + _T("\\") + m_sQMExeName;
	if ((_access(sChkPath.c_str(), 0)) != -1)
	{
        StartQM();
	}
	m_nState = 1;
	StartGuardThread();
	StartHeartbeat();
	GetSSMQMng()->SetMngCallback(this);
	return WaitForRightStatus();
}

void CQBToQMFrame::RegCallback(IQBToQMCallback* pCallback)
{
	m_pCallback = pCallback;
}

void CQBToQMFrame::CloseLocalDataBus()
{
	GetSSMQMng()->CloseLocalDataBus();
}

bool CQBToQMFrame::SelContacts(const std::string& sSelID, const std::string& sWndID/* = ""*/, int nLimit/* = 200*/, bool bShowGrp/* = true*/)
{
	std::string EncodeObj = sSelID;
	CQBToQMObjCoder::Encode_Str(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SelContacts(EncodeObj, sWndID, nLimit, bShowGrp, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SelContacts, sSelID = %s\n", EncodeObj.c_str());
	return true;
}

bool CQBToQMFrame::EditContacts(const std::string& sEditID, const LstContact& Contacts, const std::string& sWndID/* = ""*/, int nLimit/* = 200*/, bool bShowGrp/* = true*/)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::EditContacts, sEditID = %s\n", sEditID.c_str());

	std::string EncodeObj = sEditID;
	CQBToQMObjCoder::Encode_Str(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_EditContacts(EncodeObj, Contacts, sWndID, nLimit, bShowGrp, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::SendQuot(const LstQuotSendInfo& lstSend)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SendQuotV1, content size %d\n", lstSend.size());

	LstQuotSendInfo EncodeObj = lstSend;
	CQBToQMObjCoder::Encode_LstQuotSendInfo(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SendQuot(EncodeObj, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

std::string CQBToQMFrame::SendQuot(const LstMMQuotSendInfoV2& lstSend)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SendQuotV2, content size %d, ReqID = %s\n", lstSend.size(), sMsgID.c_str());

    LstMMQuotSendInfoV2 EncodeObj = lstSend;
    CQBToQMObjCoder::Encode_LstMMQuotSendInfoV2(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_SendQuotV2(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

bool CQBToQMFrame::QueryContactInfo(const LstContact& lstIn)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryContactInfo, content size %d\n", lstIn.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QueryContactsInfo(lstIn, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::QuitQM(bool bChkSession)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QuitQM\n");

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QuitQM(bChkSession, GetSessionID(), msg);
	GetSSMQMng()->Publish("qm.mng", msg);
	return true;
}

bool CQBToQMFrame::RequestBondInfosRet(const LstBondItem& LstRet)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::RequestBondInfosRet, content size %d\n", LstRet.size());

	LstBondItem EncodeObj = LstRet;
	CQBToQMObjCoder::Encode_LstBondItem(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_RequestBondInfosRet(EncodeObj, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::OpenMainPanel(const std::string& sWndID/* = ""*/)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::OpenMainPanel\n");

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_OpenMainPanel(sWndID, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

//bool CQBToQMFrame::RefreshBondInfos()
//{
//	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::RefreshBondInfos\n");
//
//	sdbus::Message msg;
//	CMsgMng::Instance()->GetMsg_RefreshBondInfos(msg);
//	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
//	return true;
//}

bool CQBToQMFrame::OpenDialogue(const Contact& ct, const string& sSendMsg, const std::string& sWndID/* = ""*/, const bool bOpen/* = true*/)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::OpenDialogue\n");

	string EncodeObj = sSendMsg;
	CQBToQMObjCoder::Encode_Str(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_OpenDialogue(ct, EncodeObj, sWndID, bOpen, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::ReplyQuot(const QuotReplyInfo& Reply, const std::string& sWndID/* = ""*/)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReplyQuot\n");

	QuotReplyInfo EncodeObj = Reply;
	CQBToQMObjCoder::Encode_QuotReplyInfo(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_ReplyQuot(EncodeObj, sWndID, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::QueryCurUsrID()
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryCurUsrID\n");

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QueryCurUsrID(msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::QueryCurUsrFriendIDs()
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryCurUsrFriendIDs\n");

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QueryCurUsrFriendIDs(msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::QueryCurUsrGroupIDs()
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryCurUsrGroupIDs\n");

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_QueryCurUsrGroupIDs(msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::SendBondQuotByCode(const LstBondQuotByCodeSendInfo& lstSend)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SentBondQuotByCodeV1, lstSend Size = %d\n", lstSend.size());

    LstBondQuotByCodeSendInfo EncodeObj = lstSend;
    CQBToQMObjCoder::Encode_LstBondQuotByCodeSendInfo(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SendBondQuotByCode(EncodeObj, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

std::string CQBToQMFrame::SendBondQuotByCode(const LstBondQuotByCodeSendInfoV2& lstSend)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SentBondQuotByCodeV2, lstSend Size = %d, ReqID = %s\n", lstSend.size(), sMsgID.c_str());

    LstBondQuotByCodeSendInfoV2 EncodeObj = lstSend;
    CQBToQMObjCoder::Encode_LstBondQuotByCodeSendInfoV2(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_SendBondQuotByCodeV2(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

bool CQBToQMFrame::SendBondQuotByCondition(const LstBondQuotByConditionSendInfo& lstSend)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SendBondQuotByCondition, lstSend Size = %d\n", lstSend.size());

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SendBondQuotByCondition(lstSend, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::ReplyBondQuotByCode(const BondQuotByCodeReplyInfo& Reply, const std::string& sWndID/* = ""*/)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReplyBondQuotByCode, ReqID = %s\n", sMsgID.c_str());

    BondQuotByCodeReplyInfo EncodeObj = Reply;
    CQBToQMObjCoder::Encode_BondQuotByCodeReplyInfo(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ReplyBondQuotByCode(EncodeObj, sWndID, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}


bool CQBToQMFrame::SendMsg(const LstContact& Contacts, const std::string& sMsg, const std::string& sWndID/* = ""*/)
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SendMsg\n");

	string EncodeObj = sMsg;
	CQBToQMObjCoder::Encode_Str(EncodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->GetMsg_SendMsg(Contacts, EncodeObj, sWndID, msg);
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
	return true;
}

bool CQBToQMFrame::QueryQMWndPos(const std::string& sWndID, int& nTop, int& nBottom, int& nLeft, int& nRight, int timeout/* = -1*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryQMWndPos\n");

    sdbus::Message msgReq;
    CMsgMng::Instance()->GetMsg_QueryQMWndPos(sWndID, msgReq);
    msgReq.SetMessageID(GetSSMQMng()->GetUniqueMsgID());

    sdbus::Message msgAck;
    bool bRet = GetSSMQMng()->Send(GetSendQueueID(msgReq), msgReq, msgAck, timeout);
    CMsgMng::Instance()->ParseMsg_QueryQMWndPosRet(msgAck, nTop, nBottom, nLeft, nRight);
    return bRet;
}

bool CQBToQMFrame::QueryQMWndHandle(const std::string& sWndID, HWND& hWnd, int timeout/* = -1*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryQMWndHandle\n");

    sdbus::Message msgReq;
    CMsgMng::Instance()->GetMsg_QueryQMWndHandle(sWndID, msgReq);
    msgReq.SetMessageID(GetSSMQMng()->GetUniqueMsgID());

    sdbus::Message msgAck;
    bool bRet = GetSSMQMng()->Send(GetSendQueueID(msgReq), msgReq, msgAck, timeout);
    CMsgMng::Instance()->ParseMsg_QueryQMWndHandleRet(msgAck, hWnd);
    return bRet;
}

int CQBToQMFrame::GetMultiLoginStatus()
{
    m_hMultiLoginMutex = CreateMutexA(NULL, FALSE, UNIQUE_MUTEXNAME_ML);
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return 1;
    }
    return 0;
}

void CQBToQMFrame::ResetMultiLoginStatus()
{
    if (m_hMultiLoginMutex)
    {
        CloseHandle(m_hMultiLoginMutex);
        m_hMultiLoginMutex = NULL;
    }
}

bool CQBToQMFrame::QuerySharedGroup(Contact& ct, int timeout/* = -1*/)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QuerySharedGroup, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msgReq;
    CMsgMng::Instance()->GetMsg_QueryShareGroup(msgReq);
    msgReq.SetMessageID(sMsgID);

    sdbus::Message msgAck;
    bool bRet = GetSSMQMng()->Send(GetSendQueueID(msgReq), msgReq, msgAck, timeout);
    CMsgMng::Instance()->ParseMsg_QueryShareGroupRet(msgAck, ct);
    return bRet;
}

std::string CQBToQMFrame::SearchQMContact(const ContactSearch& Input)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SearchQMContact, MsgID = %s\n", sMsgID.c_str());

    ContactSearch EncodeObj = Input;
    CQBToQMObjCoder::Encode_ContactSearch(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_SearchQMContact(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

void CQBToQMFrame::GetAllDealerRet(const std::string& sReqID, const SetUInt64& setID)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::GetAllDealerRet, MsgID = %s\n", sReqID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_GetAllDealerRet(setID, msg);
    msg.SetMessageID(sReqID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

std::string CQBToQMFrame::Purchase(const Contact& ct, const PurchaseInfo& PInfo, const std::string& sWndID/* = ""*/)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::Purchase, MsgID = %s\n", sMsgID.c_str());

    PurchaseInfo EncodeObj = PInfo;
    CQBToQMObjCoder::Encode_PurchaseInfo(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_Purchase(ct, EncodeObj, sWndID, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

void CQBToQMFrame::PurchaseStatusPush(const MapPurchaseStatus& mapPStatus)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::PurchaseStatusPush, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_PurchaseStatusPush(mapPStatus, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ConfirmPurchaseRet(const std::string& sReqID, int nStatus)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ConfirmPurchaseRet, MsgID = %s\n", sReqID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ConfirmPurchaseRet(nStatus, msg);
    msg.SetMessageID(sReqID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ContactIsDealerPush(const Contact& ct, bool bRet)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ContactIsDealerPush, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ContactIsDealerPush(ct, bRet, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::IsEditPurchaseValidPush(const Contact& ct, bool bRet)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::IsEditPurchaseValidPush, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_IsEditPurchaseValidPush(ct, bRet, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::QueryMMQuotStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryMMQuotStatusRet, MsgID = %s, Cnt = %d\n", sReqID.c_str(), lstRet.size());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryMMQuotStatusRet(lstRet, msg);
    msg.SetMessageID(sReqID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::QueryBondQuotByCodeStatusRet(const std::string& sReqID, const LstQuotStatus& lstRet)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryBondQuotByCodeStatusRet, MsgID = %s, Cnt = %d\n", sReqID.c_str(), lstRet.size());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryBondQuotByCodeStatusRet(lstRet, msg);
    msg.SetMessageID(sReqID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

std::string CQBToQMFrame::QueryQMIDByQBAccount(const SetString& setQBAccount)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryQMIDByQBAccount, MsgID = %s\n", sMsgID.c_str());

    SetString EncodeObj = setQBAccount;
    CQBToQMObjCoder::Encode_SetStr(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQMIDByQBAccount(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::QueryQMRemindConfig()
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryQMRemindConfig, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQMRemindConfig(msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::AddQMRemindKeyItem(const QMRemindKeyItem& Item)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::AddQMRemindKeyItem, MsgID = %s\n", sMsgID.c_str());

    QMRemindKeyItem EncodeObj = Item;
    CQBToQMObjCoder::Encode_QMRemindKeyItem(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_AddQMRemindKeyItem(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::ModifyQMRemindKeyItem(const QMRemindKeyItem& Item)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ModifyQMRemindKeyItem, MsgID = %s\n", sMsgID.c_str());

    QMRemindKeyItem EncodeObj = Item;
    CQBToQMObjCoder::Encode_QMRemindKeyItem(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ModifyQMRemindKeyItem(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::DelQMRemindKeyItem(const SetString& setID)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::DelQMRemindKeyItem, MsgID = %s\n", sMsgID.c_str());

    SetString EncodeObj = setID;
    CQBToQMObjCoder::Encode_SetStr(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_DelQMRemindKeyItem(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::AddQMRemindFriendItem(const QMRemindFriendItem& Item)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::AddQMRemindFriendItem, MsgID = %s\n", sMsgID.c_str());

    QMRemindFriendItem EncodeObj = Item;
    CQBToQMObjCoder::Encode_QMRemindFriendItem(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_AddQMRemindFriendItem(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::ModifyQMRemindFriendItem(const QMRemindFriendItem& Item)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ModifyQMRemindFriendItem, MsgID = %s\n", sMsgID.c_str());

    QMRemindFriendItem EncodeObj = Item;
    CQBToQMObjCoder::Encode_QMRemindFriendItem(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ModifyQMRemindFriendItem(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::DelQMRemindFriendItem(const SetString& setID)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::DelQMRemindFriendItem, MsgID = %s\n", sMsgID.c_str());

    SetString EncodeObj = setID;
    CQBToQMObjCoder::Encode_SetStr(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_DelQMRemindFriendItem(EncodeObj, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

std::string CQBToQMFrame::SetQMRemindType(unsigned int nRemindType)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SetQMRemindType, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_SetQMRemindType(nRemindType, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return sMsgID.c_str();
}

bool CQBToQMFrame::OpenNewsBulletin(const std::string& sWndID/* = ""*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::OpenNewsBulletin\n");

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_OpenNewsBulletin(sWndID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return true;
}

bool CQBToQMFrame::QueryCustomService(Contact& ct, int timeout/* = -1*/)
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryCustomService, MsgID = %s\n", sMsgID.c_str());

    sdbus::Message msgReq;
    CMsgMng::Instance()->GetMsg_QueryCustomService(msgReq);
    msgReq.SetMessageID(sMsgID);

    sdbus::Message msgAck;
    if (GetSSMQMng()->Send(GetSendQueueID(msgReq), msgReq, msgAck, timeout))
    {
        CMsgMng::Instance()->ParseMsg_QueryCustomServiceRet(msgAck, ct);
        return true;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryCustomService Timeout! MsgID = %s\n", sMsgID.c_str()); 
    return false;
}

void CQBToQMFrame::QBModifyPwd(const std::string& sNewPwd)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QBModifyPwd\n");

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QBModifyPwd(sNewPwd, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

bool CQBToQMFrame::ShareBond(const LstContact& Contacts, const LstBondShare& lstBS, const std::string& sWndID/* = ""*/)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ShareBond\n");

    LstBondShare EncodeObj = lstBS;
    CQBToQMObjCoder::Encode_LstBondShare(EncodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ShareBond(Contacts, EncodeObj, sWndID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return true;
}

void CQBToQMFrame::SetQMStatus(int nStatus, string sSessionID)
{
	if (sSessionID != GetSessionID())
	{
		//QM可以启动多个,避免干扰
		return;
	}

	if (m_bStopGuard)
	{
		StopGuardThread();
		StartGuardThread();
	}
	SetEvent(m_hHeartbeatEvent);

	if (m_nQMStatus == nStatus)
	{
		return;
	}
	
	m_nQMStatus = nStatus;
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "QMStatus Update, Status = %d\n", m_nQMStatus);
    if (m_pCallback)
    {
        m_pCallback->QMStatus(m_nQMStatus);
    }
}

string CQBToQMFrame::GetSessionID()
{
	string sRet = m_sUsr;
	sRet += "$$";
	sRet += m_sPwd;

	MD5 EncodeTool;
	EncodeTool.update(sRet);
	return EncodeTool.toString();
}

bool CQBToQMFrame::SelContactsRet(const std::string& sSelID, const LstContact& Contacts)
{
	string DecodeObj = sSelID;
	CQBToQMObjCoder::Decode_Str(DecodeObj);

	if (!m_pCallback)
	{
		return false;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SelContactsRet, sSelID = %s\n", DecodeObj.c_str());
	return m_pCallback->SelContactsRet(DecodeObj, Contacts);
}

bool CQBToQMFrame::QueryContactInfoRet(LstPersonInfo& PersonInfos, LstGroupInfo& GrpInfos, const LstContact& lstFailed)
{
	LstPersonInfo DecodeObj1 = PersonInfos;
	CQBToQMObjCoder::Decode_LstPersonInfo(DecodeObj1);
	LstGroupInfo DecodeObj2 = GrpInfos;
	CQBToQMObjCoder::Decode_LstGroupInfo(DecodeObj2);

	if (!m_pCallback)
	{
		return false;
	}
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryContactInfoRet, LstPerson size : %d, LstGroup size : %d, LstFailed size : %d\n", PersonInfos.size(), GrpInfos.size(), lstFailed.size());
	return m_pCallback->QueryContactInfoRet(DecodeObj1, DecodeObj2, lstFailed);
}

bool CQBToQMFrame::EditContactsRet(const std::string& sEditID, const LstContact& Contacts)
{
	std::string DecodeObj = sEditID;
	CQBToQMObjCoder::Decode_Str(DecodeObj);

	if (!m_pCallback)
	{
		return false;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::EditContactsRet, sEditID = %s\n", sEditID.c_str());
	return m_pCallback->EditContactsRet(DecodeObj, Contacts);
}

bool CQBToQMFrame::SendQuotRet(LstQuotSendInfo& lstSuccess, LstQuotSendInfo& lstFailed)
{
	LstQuotSendInfo DecodeObj1 = lstSuccess;
	CQBToQMObjCoder::Decode_LstQuotSendInfo(DecodeObj1);
	LstQuotSendInfo DecodeObj2 = lstFailed;
	CQBToQMObjCoder::Decode_LstQuotSendInfo(DecodeObj2);

	if (!m_pCallback)
	{
		return false;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SendQuotRetV1, Success Num = %d, Failed Num = %d\n", lstSuccess.size(), lstFailed.size());
	//return m_pCallback->SendQuotRet(DecodeObj1, DecodeObj2);
    return false;
}

bool CQBToQMFrame::RequestBondInfos()
{
	if (!m_pCallback)
	{
		return false;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::RequestBondInfos\n");
	return m_pCallback->RequestBondInfos();
}

bool CQBToQMFrame::ShowDlgBondDetail(const std::string& sBondKey)
{
	std::string DecodeObj = sBondKey;
	CQBToQMObjCoder::Decode_Str(DecodeObj);

	if (!m_pCallback)
	{
		return false;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ShowDlgBondDetail, sBondKey = %s\n", sBondKey.c_str());
	return m_pCallback->ShowDlgBondDetail(DecodeObj);
}

bool CQBToQMFrame::RecvQuot(const LstQuotRecvInfo& lstRecv)
{
	//LstQuotRecvInfo DecodeObj = lstRecv;
	//CQBToQMObjCoder::Decode_LstQuotRecvInfo(DecodeObj);
	//if (!m_pCallback)
	//{
	//	return false;
	//}

	//CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::RecvQuot, lstRecv Size = %d\n", lstRecv.size());
	//return m_pCallback->RecvQuot(DecodeObj);
	return false;
}

void CQBToQMFrame::QueryCurUsrIDRet(unsigned long long UsrID)
{
	if (!m_pCallback)
	{
		return;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryCurUsrIDRet\n");
	m_pCallback->QueryCurUsrIDRet(UsrID);
}

bool CQBToQMFrame::SendBondQuotByCodeRet(const LstBondQuotByCodeSendInfo& lstSuccess, const LstBondQuotByCodeSendInfo& lstFailed)
{
	LstBondQuotByCodeSendInfo DecodeObj1 = lstSuccess;
	CQBToQMObjCoder::Decode_LstBondQuotByCodeSendInfo(DecodeObj1);
	LstBondQuotByCodeSendInfo DecodeObj2 = lstFailed;
	CQBToQMObjCoder::Decode_LstBondQuotByCodeSendInfo(DecodeObj2);

	if (!m_pCallback)
	{
		return false;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SentBondQuotByCodeRetV1, Success Num = %d, Failed Num = %d\n", lstSuccess.size(), lstFailed.size());
	//return m_pCallback->SendBondQuotByCodeRet(DecodeObj1, DecodeObj2);
    return false;
}

bool CQBToQMFrame::SendBondQuotByConditionRet(const LstBondQuotByConditionSendInfo& lstSuccess, const LstBondQuotByConditionSendInfo& lstFailed)
{
	LstBondQuotByConditionSendInfo DecodeObj1 = lstSuccess;
	CQBToQMObjCoder::Decode_LstBondQuotByConditionSendInfo(DecodeObj1);
	LstBondQuotByConditionSendInfo DecodeObj2 = lstFailed;
	CQBToQMObjCoder::Decode_LstBondQuotByConditionSendInfo(DecodeObj2);

	if (!m_pCallback)
	{
		return false;
	}
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SendBondQuotByConditionRet, Success Num = %d, Failed Num = %d\n", lstSuccess.size(), lstFailed.size());
	//return m_pCallback->SendBondQuotByConditionRet(DecodeObj1, DecodeObj2);
    return false;
}

void CQBToQMFrame::QueryCurUsrFriendIDsRet(const SetUInt64& setIDs)
{
	if (!m_pCallback)
	{
		return;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryCurUsrFriendIDsRet, setIDs Size = %d\n", setIDs.size());
	m_pCallback->QueryCurUsrFriendIDsRet(setIDs);
}

void CQBToQMFrame::QueryCurUsrGroupIDsRet(const SetUInt64& setIDs)
{
	if (!m_pCallback)
	{
		return;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryCurUsrGroupIDsRet, setIDs Size = %d\n", setIDs.size());
	m_pCallback->QueryCurUsrGroupIDsRet(setIDs);
}

void CQBToQMFrame::CurUsrGroupChange(const SetUInt64& JoinGrpIDs, const SetUInt64& LeaveGrpIDs)
{
	if (!m_pCallback)
	{
		return;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::CurUsrGroupChange, JoinGrpIDs Size = %d, LeaveGrpIDs Size = %d\n", JoinGrpIDs.size(), LeaveGrpIDs.size());
	m_pCallback->CurUsrGroupChange(JoinGrpIDs, LeaveGrpIDs);
}

void CQBToQMFrame::CurUsrFriendChange(const SetUInt64& AddFriendIDs, const SetUInt64& DelFriendIDs)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::CurUsrFriendChange, AddFriendIDs Size = %d, DelFriendIDs Size = %d\n", AddFriendIDs.size(), DelFriendIDs.size());
    m_pCallback->CurUsrFriendChange(AddFriendIDs, DelFriendIDs);
}

void CQBToQMFrame::GroupDisband(unsigned long long GrpID)
{
	if (!m_pCallback)
	{
		return;
	}

	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::GroupDisband\n");
	//m_pCallback->GroupDisband(GrpID);
}

bool CQBToQMFrame::QueryQBWndPos(const std::string& sMsgID, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryQBWndPos\n");

    int nTop(0), nBottom(0), nLeft(0), nRight(0);
    bool bRet = m_pCallback->QueryQBWndPos(sWndID, nTop, nBottom, nLeft, nRight);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQBWndPosRet(nTop, nBottom, nLeft, nRight, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

bool CQBToQMFrame::QueryQBWndHandle(const std::string& sMsgID, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryQBWndHandle\n");

    HWND hWnd(0);
    bool bRet = m_pCallback->QueryQBWndHandle(sWndID, hWnd);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQBWndHandleRet(hWnd, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

bool CQBToQMFrame::QueryQBPerm(const std::string& sMsgID)
{
    if (!m_pCallback)
    {
        return false;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryQBPerm\n");

    SetString setPerm;
    bool bRet = m_pCallback->QueryQBPerm(setPerm);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryQBPermRet(setPerm, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
    return bRet;
}

void CQBToQMFrame::OpenMMQuot(const LstContact& Contacts, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::OpenMMQuot\n");
    m_pCallback->OpenMMQuot(Contacts, sWndID);
}

void CQBToQMFrame::SearchQMContactRet(const std::string& sMsgID, const LstSearchUserInfo& Result)
{
    if (!m_pCallback)
    {
        return;
    }

    LstSearchUserInfo DecodeObj = Result;
    CQBToQMObjCoder::Decode_LstSearchUserInfo(DecodeObj);

   CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SearchQMContactRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->SearchQMContactRet(sMsgID, DecodeObj);
}

void CQBToQMFrame::QueryContactIsDealer(const std::string& sMsgID, const Contact& ct)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryContactIsDealer\n");

    bool bRet = m_pCallback->ContactIsDealer(ct);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_ContactIsDealerRet(bRet, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::OpenSubscribeDlg(const std::string& sMsgID, const Contact& ct, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::OpenSubscribeDlg\n");
    m_pCallback->OpenSubscribeDlg(ct, sWndID);
}

void CQBToQMFrame::GetAllDealer(const std::string& sMsgID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::GetAllDealer\n");

    SetUInt64 setID;
    m_pCallback->GetAllDealer(sMsgID);
}

bool CQBToQMFrame::SendMMQuotRetV2(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed)
{
    if (!m_pCallback)
    {
        return false;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SendMMQuotRetV2, Success Num = %d, Failed Num = %d, ReqID = %s\n", Success.size(), Failed.size(), sMsgID.c_str());
    return m_pCallback->SendQuotRet(sMsgID, ct, Success, Failed);
}

bool CQBToQMFrame::SendBondQuotByCodeRetV2(const std::string& sMsgID, const Contact& ct, const SetString& Success, const SetString& Failed)
{
    if (!m_pCallback)
    {
        return false;
    }
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SendBondQuotByCodeRetV2, Success Num = %d, Failed Num = %d, ReqID = %s\n", Success.size(), Failed.size(), sMsgID.c_str());
    return m_pCallback->SendBondQuotByCodeRet(sMsgID, ct, Success, Failed);
}

void CQBToQMFrame::QMErrInfo(int nErrCode, const std::string& sErrInfo)
{
    if (!m_pCallback)
    {
        return;
    }

    string sDecodeInfo = sErrInfo;
    CQBToQMObjCoder::Decode_Str(sDecodeInfo);

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QMErrInfo!\n");
    m_pCallback->QMErrInfo(nErrCode, sDecodeInfo);
}

void CQBToQMFrame::QueryPurchaseStatus(const std::string& sMsgID, const LstPurchaseUkey& lstQuery)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryPurchaseStatus, MsgID = %s\n", sMsgID.c_str());

    MapPurchaseStatus mapPStatus;
    m_pCallback->QueryPurchaseStatus(lstQuery, mapPStatus);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_QueryPurchaseStatusRet(mapPStatus, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ConfirmPurchase(const std::string& sMsgID, const std::string& sPurchaseID, time_t ModifyTime)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ConfirmPurchase, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->ConfirmPurchase(sMsgID, sPurchaseID, ModifyTime);
}

void CQBToQMFrame::IsEditPurchaseValid(const std::string& sMsgID, const Contact& ct)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::IsEditPurchaseValid, MsgID = %s\n", sMsgID.c_str());
    bool bValid = m_pCallback->IsEditPurchaseValid(ct);

    sdbus::Message msg;
    CMsgMng::Instance()->GetMsg_IsEditPurchaseValidRet(bValid, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::EditPurchase(const std::string& sMsgID, const Contact& ct, const std::string& sWndID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::EditPurchase, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->EditPurchase(ct, sWndID);
}

void CQBToQMFrame::QueryMMQuotStatus(const std::string& sMsgID, const LstQuotStatusReq& lstReq)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryMMQuotStatus, MsgID = %s, Cnt = %d\n", sMsgID.c_str(), lstReq.size());
    m_pCallback->QueryMMQuotStatus(sMsgID, lstReq);
}

void CQBToQMFrame::QueryBondQuotByCodeStatus(const std::string& sMsgID, const LstQuotStatusReq& lstReq)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryBondQuotByCodeStatus, MsgID = %s, Cnt = %d\n", sMsgID.c_str(), lstReq.size());

    m_pCallback->QueryBondQuotByCodeStatus(sMsgID, lstReq);
}

void CQBToQMFrame::ContactInfoPush(const std::string& sMsgID, const LstPersonInfo& PersonInfos, const LstGroupInfo& GrpInfos)
{
    if (!m_pCallback)
    {
        return;
    }

    LstPersonInfo DecodeObj1 = PersonInfos;
    CQBToQMObjCoder::Decode_LstPersonInfo(DecodeObj1);
    LstGroupInfo DecodeObj2 = GrpInfos;
    CQBToQMObjCoder::Decode_LstGroupInfo(DecodeObj2);

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ContactInfoPush, MsgID = %s, PersonInfo Cnt = %d, GrpInfo Cnt = %d\n", sMsgID.c_str(), DecodeObj1.size(), DecodeObj2.size());

    m_pCallback->ContactInfoPush(DecodeObj1, DecodeObj2);
}

void CQBToQMFrame::ContactStatusPush(const std::string& sMsgID, const LstContactStatus& lstStatus)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ContactStatusPush, MsgID = %s, Cnt = %d\n", sMsgID.c_str(), lstStatus.size());

    m_pCallback->ContactStatusPush(lstStatus);
}

void CQBToQMFrame::QueryQMIDByQBAccountRet(const std::string& sMsgID, const LstQBAccount2QMID& lstRet)
{
    if (!m_pCallback)
    {
        return;
    }

    LstQBAccount2QMID DecodeObj = lstRet;
    CQBToQMObjCoder::Decode_LstQBAccount2QMID(DecodeObj);
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryQMIDByQBAccountRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->QueryQMIDByQBAccountRet(sMsgID, DecodeObj);
}

void CQBToQMFrame::QueryQMRemindConfigRet(const std::string& sMsgID, const QMRemindConfig& QMRCfg)
{
    if (!m_pCallback)
    {
        return;
    }

    QMRemindConfig DecodeObj = QMRCfg;
    CQBToQMObjCoder::Decode_QMRemindConfig(DecodeObj);
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryQMRemindConfigRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->QueryQMRemindConfigRet(sMsgID, DecodeObj);
}

void CQBToQMFrame::AddQMRemindKeyItemRet(const std::string& sMsgID, const std::string& ItemID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::AddQMRemindKeyItemRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->AddQMRemindKeyItemRet(sMsgID, ItemID);
}

void CQBToQMFrame::ModifyQMRemindKeyItemRet(const std::string& sMsgID, bool bSuccess)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ModifyQMRemindKeyItemRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->ModifyQMRemindKeyItemRet(sMsgID, bSuccess);
}

void CQBToQMFrame::DelQMRemindKeyItemRet(const std::string& sMsgID, const SetString& setSuccessID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::DelQMRemindKeyItemRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->DelQMRemindKeyItemRet(sMsgID, setSuccessID);
}

void CQBToQMFrame::AddQMRemindFriendItemRet(const std::string& sMsgID, const std::string& ItemID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::AddQMRemindFriendItemRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->AddQMRemindFriendItemRet(sMsgID, ItemID);
}

void CQBToQMFrame::ModifyQMRemindFriendItemRet(const std::string& sMsgID, bool bSuccess)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ModifyQMRemindFriendItemRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->ModifyQMRemindFriendItemRet(sMsgID, bSuccess);
}

void CQBToQMFrame::DelQMRemindFriendItemRet(const std::string& sMsgID, const SetString& setSuccessID)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::DelQMRemindFriendItemRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->DelQMRemindFriendItemRet(sMsgID, setSuccessID);
}

void CQBToQMFrame::SetQMRemindTypeRet(const std::string& sMsgID, bool bSuccess)
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::SetQMRemindTypeRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->SetQMRemindTypeRet(sMsgID, bSuccess);
}

bool CQBToQMFrame::WaitforHeartbeat()
{
	if (WAIT_TIMEOUT == WaitForSingleObject(m_hHeartbeatEvent, 1500))
	{
		return false;
	}
	return true;
}

void CQBToQMFrame::HBTimeout()
{
	m_nQMStatus = 0;
	if (m_pCallback)
	{
        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "QM Heartbeat Timeout! QMStatus Update, Status = %d\n", m_nQMStatus);
		m_pCallback->QMStatus(m_nQMStatus);
	}
}

void CQBToQMFrame::InitObs()
{
	ClearObs();
	m_LstObs.push_back(new CQBObs_QMStatus());
	//m_LstObs.push_back(new CQBObs_Session());
	m_LstObs.push_back(new CQBObs_Session1());
	m_LstObs.push_back(new CQBObs_Session2());
	m_LstObs.push_back(new CQBObs_Session3());
	m_LstObs.push_back(new CQBObs_Session4());
	m_LstObs.push_back(new CQBObs_Session5());
    m_LstObs.push_back(new CQBObs_Session6());
    m_LstObs.push_back(new CQBObs_Session7());
    m_LstObs.push_back(new CQBObs_Session8());
    m_LstObs.push_back(new CQBObs_Session9());

	for (LstObs::iterator iter = m_LstObs.begin(); iter != m_LstObs.end(); ++iter)
	{
		(*iter)->SetCommFrame(this);
		GetSSMQMng()->RegObs(*iter);
	}
}

void CQBToQMFrame::StartQM()
{
	//用户名和密码直接通过启动参数传入
	int nProcCnt = FindProcessByName(m_sQMExeName);
	if (nProcCnt > 0)
	{
		QuitQM(true);

		int nCheckCount = 30;
		while (FindProcessByName(m_sQMExeName) >= nProcCnt)
		{
			Sleep(100);
			nCheckCount --;
			if (nCheckCount <= 0)
			{
				break;
			}
		}
	}

	t_string sParam(_T(""));
	sParam += SS_BASE::char2t_string(m_sUsr.c_str());
	sParam += _T(" ");
	sParam += SS_BASE::char2t_string(m_sPwd.c_str());
    if (0 != m_stQMProxyInfo.ProxyType)
    {
        char buf[256];
        memset(buf, 0, 256);
        sprintf_s(buf, 256, "ProxyType:%d;ProxyIp:%s;ProxyPort:%s;ProxyUser:%s;ProxyPass:%s;ProxyDomain:%s", 
            m_stQMProxyInfo.ProxyType, m_stQMProxyInfo.ProxyIP.c_str(), m_stQMProxyInfo.ProxyPort.c_str(), m_stQMProxyInfo.ProxyUser.c_str(), 
            m_stQMProxyInfo.ProxyPwd.c_str(), m_stQMProxyInfo.ProxyDomain.c_str());

        CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "ProxyInfo = %s\n", buf);
        sParam += _T(" ");
        sParam += SS_BASE::char2t_string(buf);
    }


	SHELLEXECUTEINFO ShExecInfo = {0}; 
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShExecInfo.hwnd = NULL; 
	ShExecInfo.lpVerb = NULL; 
	ShExecInfo.lpFile = m_sQMExeName.c_str();
	ShExecInfo.lpParameters = sParam.c_str(); 
	ShExecInfo.lpDirectory = m_sQMDir.c_str(); 
	ShExecInfo.nShow = SW_SHOW; 
	ShExecInfo.hInstApp = NULL; 
	ShellExecuteEx(&ShExecInfo);
}

bool CQBToQMFrame::WaitForRightStatus()
{
	if (1 == m_nQMStatus)
	{
		return true;
	}

	int nWaitCnt = 6;
	while (1 != m_nQMStatus)
	{
		if (nWaitCnt <= 0)
		{
			return false;
		}
		Sleep(200);
		nWaitCnt --;
	}
	return true;
}

void CQBToQMFrame::ClearObs()
{
    for (LstObs::iterator iter = m_LstObs.begin(); iter != m_LstObs.end(); ++iter)
    {
        GetSSMQMng()->ReleaseObs(*iter);
    }
	ss_tool::ClearContainer(m_LstObs);
}

string CQBToQMFrame::GetSendQueueID(const sdbus::Message& msgs)
{
	static CMsgQueueMap mapTool;
	string sRet = mapTool.GetQueueHead(msgs);
	sRet += ".";
	sRet += GetSessionID();
	return sRet;
}

int CQBToQMFrame::FindProcessByName(t_string sFileName)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return FALSE;

	cProcesses = cbNeeded / sizeof(DWORD);

	TCHAR szProcessName[MAX_PATH] = _T("<unknown>");
	HANDLE hProcess;
	t_string strName;
	int nRuningProc = 0;
	for (unsigned int i = 0; i < cProcesses; i++)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

		if (NULL != hProcess)
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
				::GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
				strName = szProcessName;
				if (SS_BASE::CompareNoCase(sFileName, strName) == 0)
				{
					nRuningProc ++;
					CloseHandle(hProcess);
					continue;
				}
			}
			CloseHandle(hProcess);
		}
	}
	return nRuningProc;
}

t_string CQBToQMFrame::GetModuleDirPath()
{
	TCHAR szFileNames[260];
	DWORD dwLen = GetModuleFileName(NULL, szFileNames, sizeof(szFileNames));
	for(DWORD offset=dwLen; offset>=0; offset--)
	{
		if(szFileNames[offset] == _T('\\'))
		{
			szFileNames[offset] = _T('\0');
			break;
		}
	}

	t_string sRet = szFileNames;
	sRet += _T("\\");
	return sRet;
}


unsigned int __stdcall GuardThread(PVOID pM)
{
	unsigned int nRet = 0;
	CQBToQMFrame *pThis = (CQBToQMFrame*)pM;
	while (!pThis->m_bStopGuard)
	{
		if (!pThis->WaitforHeartbeat())
		{
			int nLimitCnt = 0;//心跳容错次数
			bool bAlive = false;
			while (nLimitCnt < 3 && !pThis->m_bStopGuard)
			{
				nLimitCnt++;
				if (pThis->WaitforHeartbeat())
				{
					bAlive = true;
					break;
				}
			}
			if (bAlive)
			{
				continue;
			}
			pThis->HBTimeout();
			pThis->m_bStopGuard = true;
		}
	}

	return nRet;
}

void CQBToQMFrame::StartGuardThread()
{
	m_bStopGuard = false;
	m_hGuardThread = (HANDLE)_beginthreadex(NULL, 0, GuardThread, this, 0, NULL);
}

void CQBToQMFrame::StopGuardThread()
{
	m_bStopGuard = true;
	if (m_hGuardThread)
	{
		WaitForSingleObject(m_hGuardThread, 8000);
		CloseHandle(m_hGuardThread);
		m_hGuardThread = NULL;
	}
}


void CQBToQMFrame::Heartbeat()
{
	sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(1, "heartbeat");
	values.SetInt32(2, m_nState);
	values.SetString(3, GetSessionID());

	GetSSMQMng()->Publish("qb.status", msg);
}

void CQBToQMFrame::HeartbeatThread()
{
	while (!m_bStopHeartbeat)
	{
		Heartbeat();
		int n = 5;
		while (n > 1)
		{
			if (m_bStopHeartbeat)
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
}

void CQBToQMFrame::StartHeartbeat()
{
	if (m_ThreadHeartbeat)
	{
		return;
	}
	m_bStopHeartbeat = false;
	m_ThreadHeartbeat = new boost::thread(boost::bind(&CQBToQMFrame::HeartbeatThread, this));
}

void CQBToQMFrame::StopHeartbeat()
{
	if (m_ThreadHeartbeat)
	{
		m_bStopHeartbeat = true;
		m_ThreadHeartbeat->join();
		delete m_ThreadHeartbeat;
		m_ThreadHeartbeat = NULL;
	}
}

std::string CQBToQMFrame::ReqJoinGroup( const unsigned long long &llGroup )
{
    sdbus::string sMsgID = GetSSMQMng()->GetUniqueMsgID();

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReqJoinGroup\n");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqJoinGroup(llGroup, msg);
    msg.SetMessageID(sMsgID);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);

    return sMsgID.c_str();
}

void CQBToQMFrame::ReqJoinGroupRet( const std::string& sMsgID, bool bSuccess )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ReqJoinGroupRet, MsgID = %s\n", sMsgID.c_str());
    m_pCallback->ReqJoinGroupRet(sMsgID, bSuccess);
}

void CQBToQMFrame::ReqOpenNews( const NewsInfo &info )
{
	CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReqOpenNews\n");

	NewsInfo encodeObj = info;
	CQBToQMObjCoder::Encode_NewsInfo(encodeObj);

	sdbus::Message msg;
	CMsgMng::Instance()->BuildReqOpenNews(encodeObj, msg);	
	GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ReqOpenNewsDetail( const NewsInfoV2 &info, bool bDisablePDF /*= true*/ )
{
    if (!m_pCallback)
    {
        return;
    }

    NewsInfoV2 dcodeInfo = info;
    CQBToQMObjCoder::Decode_NewsInfoV2(dcodeInfo);

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ReqOpenNewsDetail\n");
    m_pCallback->ReqOpenNewsDetail(dcodeInfo, bDisablePDF);
}

void CQBToQMFrame::SharePic( const PicInfo &info, const LstContact& Contacts )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::SharePic\n");

    PicInfo encodeObj = info;
    CQBToQMObjCoder::Encode_PicInfo(encodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->BuildSharePic(encodeObj, Contacts, msg);	
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ReqOpenNewsWnd()
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ReqOpenNewsWnd\n");
    m_pCallback->ReqOpenNewsWnd();
}

void CQBToQMFrame::ShareNewsDetail( const NewsInfoV2 &info, const LstContact& Contacts, bool bDisablePDF /*= true*/ )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ShareNewsDetail\n");

    NewsInfoV2 encodeObj = info;
    CQBToQMObjCoder::Encode_NewsInfoV2(encodeObj);

    sdbus::Message msg;
    CMsgMng::Instance()->BuildShareNewsDetail(encodeObj, Contacts, bDisablePDF, msg);	
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::OpenDialogueByUserID( const std::string &userID )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::OpenDialogueByUserID\n");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildOpenDialogueByUserID(userID, msg);	
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::QueryQMStatus()
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::QueryQMStatus\n");

    sdbus::Message msg;
    msg.SetString(1, MSGCMD_QueryQMStatus);

    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::QueryQMStatusRet( int status )
{
    if (!m_pCallback)
    {
        return;
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::QueryQMStatusRet, status=%d\n", status);
    m_pCallback->QueryQMStatusRet(status);
}

void CQBToQMFrame::ReqOpenSellsWnd( const std::string& sWndID/* = ""*/ )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReqOpenSellsWnd\n");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqOpenSellsWnd(sWndID, msg);	
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ReqQueryNoInstituteContact( LstNoInstituteContact &contacts, int &status, int timeout/* = -1 */)
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReqQueryNoInstituteContact\n");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqQueryNoInstituteContact(msg);
    msg.SetMessageID(GetSSMQMng()->GetUniqueMsgID());

    sdbus::Message msgAck;
    GetSSMQMng()->Send(GetSendQueueID(msg), msg, msgAck, timeout);

    LstNoInstituteContact tempContacts;
    CMsgMng::Instance()->ParseReqQueryNoInstituteContactRet(msgAck, tempContacts, status);

    for (LstNoInstituteContact::iterator it = tempContacts.begin(); it != tempContacts.end(); ++it)
    {
        CQBToQMObjCoder::Decode_NoInstituteContact(*it);
    }
    contacts = tempContacts;
}

void CQBToQMFrame::ReqSetNoInstituteContact( const std::list<std::string> &QBAccounts )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReqSetNoInstituteContact\n");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqSetNoInstituteContact(QBAccounts, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ReqOpenSellsSearchWnd( const std::string& sWndID/* = ""*/ )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReqOpenSellsSearchWnd\n");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqOpenSellsSearchWnd(sWndID, msg);
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}

void CQBToQMFrame::ModifyNoInstituteContactRet( const LstNoInstituteContact& contacts )
{
    if (!m_pCallback)
    {
        return;
    }

    LstNoInstituteContact DecodeObj1 = contacts;
    for (LstNoInstituteContact::iterator it = DecodeObj1.begin(); it != DecodeObj1.end(); ++it)
    {
        CQBToQMObjCoder::Decode_NoInstituteContact(*it);
    }

    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "callback CQBToQMFrame::ModifyNoInstituteContactRet, contacts size=%d\n", contacts.size());

    m_pCallback->ModifyNoInstituteContactRet(DecodeObj1);
}

void CQBToQMFrame::ReqOpenDialogueFromAd( const std::string &userID )
{
    CURLOGGERWRITEWITHLV(m_pLogger, ss::LOG_LEVEL::LOG_INFO, "call CQBToQMFrame::ReqOpenDialogueFromAd\n");

    sdbus::Message msg;
    CMsgMng::Instance()->BuildReqOpenDialogueFromAd(userID, msg);	
    GetSSMQMng()->Post(GetSendQueueID(msg), msg);
}
