#include "stdafx.h"
#include "QBObs_Session.h"
#include "MsgMng.h"

CQBObs_Session::CQBObs_Session(void)
{
}

CQBObs_Session::~CQBObs_Session(void)
{
}

sdbus::string CQBObs_Session::GetBindKey()
{
	string sRet = "qb.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

SSMQ::SSMQDeliverType CQBObs_Session::GetDeliverType()
{
	return SSMQ::SSMQDT_Direct;
}

void CQBObs_Session::OnMsgs(const sdbus::Message& msgs)
{
    std::string sMsgID = msgs.GetMessageID();
	const sdbus::Message::Map& values = msgs.MapValue();
	sdbus::string sRecv;
	values.GetString(1, sRecv);
	if (MSGCMD_MM_SelContactsRet == sRecv)
	{
		std::string sSelID;
		LstContact lstCts;
		CMsgMng::Instance()->ParseMsg_SelContactsRet(msgs, sSelID, lstCts);
		m_pCommFrame->SelContactsRet(sSelID, lstCts);
	}
	else if (MSGCMD_MM_QueryContactInfoRet == sRecv)
	{
		LstPersonInfo lstPerson;
		LstGroupInfo lstGrp;
        LstContact lstFailed;
		CMsgMng::Instance()->ParseMsg_QueryContactsInfoRet(msgs, lstPerson, lstGrp, lstFailed);
		m_pCommFrame->QueryContactInfoRet(lstPerson, lstGrp, lstFailed);
	}
	else if (MSGCMD_MM_EditContactsRet == sRecv)
	{
		std::string sEditID;
		LstContact lstCt;
		CMsgMng::Instance()->ParseMsg_EditContactsRet(msgs, sEditID, lstCt);
		m_pCommFrame->EditContactsRet(sEditID, lstCt);
	}
	else if (MSGCMD_MM_SendQuotRet == sRecv)
	{
		LstQuotSendInfo lstSuccess;
		LstQuotSendInfo lstFailed;
		CMsgMng::Instance()->ParseMsg_SendQuotRet(msgs, lstSuccess, lstFailed);
		m_pCommFrame->SendQuotRet(lstSuccess, lstFailed);
	}
	else if (MSGCMD_RequestBondInfos == sRecv)
	{
		m_pCommFrame->RequestBondInfos();
	}
	else if (MSGCMD_ShowDlgBondDetail == sRecv)
	{
		std::string sBondKey;
		CMsgMng::Instance()->ParseMsg_ShowDlgBondDetail(msgs, sBondKey);
		m_pCommFrame->ShowDlgBondDetail(sBondKey);
	}
	else if (MSGCMD_BOND_SendQuotByCodeRet == sRecv)
	{
		LstBondQuotByCodeSendInfo lstSuccess;
		LstBondQuotByCodeSendInfo lstFailed;
		CMsgMng::Instance()->ParseMsg_SendBondQuotByCodeRet(msgs, lstSuccess, lstFailed);
		m_pCommFrame->SendBondQuotByCodeRet(lstSuccess, lstFailed);
	}
	else if (MSGCMD_BOND_SendQuotByConditionRet == sRecv)
	{
		LstBondQuotByCodeSendInfo lstSuccess;
		LstBondQuotByCodeSendInfo lstFailed;
		CMsgMng::Instance()->ParseMsg_SendBondQuotByCodeRet(msgs, lstSuccess, lstFailed);
		m_pCommFrame->SendBondQuotByCodeRet(lstSuccess, lstFailed);
	}
	else if (MSGCMD_MM_RecvQuot == sRecv)
	{
		LstQuotRecvInfo Recv;
		CMsgMng::Instance()->ParseMsg_RecvQuot(msgs, Recv);
		m_pCommFrame->RecvQuot(Recv);
	}
	else if (MSGCMD_QueryCurUsrIDRet == sRecv)
	{
		unsigned long long llID = 0;
		CMsgMng::Instance()->ParseMsg_QueryCurUsrIDRet(msgs, llID);
		m_pCommFrame->QueryCurUsrIDRet(llID);
	}
	else if (MSGCMD_QueryCurUsrFriendIDsRet == sRecv)
	{
		SetUInt64 setRet;
		CMsgMng::Instance()->ParseMsg_QueryCurUsrFriendIDsRet(msgs, setRet);
		m_pCommFrame->QueryCurUsrFriendIDsRet(setRet);
	}
	else if (MSGCMD_QueryCurUsrGroupIDsRet == sRecv)
	{
		SetUInt64 setRet;
		CMsgMng::Instance()->ParseMsg_QueryCurUsrGroupIDsRet(msgs, setRet);
		m_pCommFrame->QueryCurUsrGroupIDsRet(setRet);
	}
	else if (MSGCMD_CurUsrGroupChange == sRecv)
	{
		SetUInt64 setJoin;
		SetUInt64 setLeave;
		CMsgMng::Instance()->ParseMsg_CurUsrGroupChange(msgs, setJoin, setLeave);
		m_pCommFrame->CurUsrGroupChange(setJoin, setLeave);
	}
    else if (MSGCMD_CurUsrFriendChange == sRecv)
    {
        SetUInt64 setAdd;
        SetUInt64 setDel;
        CMsgMng::Instance()->ParseMsg_CurUsrFriendChange(msgs, setAdd, setDel);
        m_pCommFrame->CurUsrFriendChange(setAdd, setDel);
    }
	else if (MSGCMD_GroupDisband == sRecv)
	{
		unsigned long long GrpID(0);
		CMsgMng::Instance()->ParseMsg_GroupDisband(msgs, GrpID);
		m_pCommFrame->GroupDisband(GrpID);
	}
    else if (MSGCMD_QueryQBWndPos == sRecv)
    {
        std::string sWndID;
        CMsgMng::Instance()->ParseMsg_QueryQBWndPos(msgs, sWndID);
        m_pCommFrame->QueryQBWndPos(sMsgID, sWndID);
    }
    else if (MSGCMD_QueryQBWndHandle == sRecv)
    {
        std::string sWndID;
        CMsgMng::Instance()->ParseMsg_QueryQBWndHandle(msgs, sWndID);
        m_pCommFrame->QueryQBWndHandle(sMsgID, sWndID);
    }
    else if (MSGCMD_QueryQBPerm == sRecv)
    {
        m_pCommFrame->QueryQBPerm(sMsgID);
    }
    else if (MSGCMD_OpenMMQuot == sRecv)
    {
        LstContact lstCt;
        std::string sWndID("");
        CMsgMng::Instance()->ParseMsg_OpenMMQuot(msgs, lstCt, sWndID);
        m_pCommFrame->OpenMMQuot(lstCt, sWndID);
    }
    else if (MSGCMD_SearchQMContactRet == sRecv)
    {
        LstSearchUserInfo lstRet;
        CMsgMng::Instance()->ParseMsg_SearchQMContactRet(msgs, lstRet);
        m_pCommFrame->SearchQMContactRet(sMsgID, lstRet);
    }
    else if (MSGCMD_ContactIsDealer == sRecv)
    {
        Contact ct;
        CMsgMng::Instance()->ParseMsg_ContactIsDealer(msgs, ct);
        m_pCommFrame->QueryContactIsDealer(sMsgID, ct);
    }
    else if (MSGCMD_OpenSubscribeDlg == sRecv)
    {
        Contact ct;
        std::string sWndID("");
        CMsgMng::Instance()->ParseMsg_OpenSubscribeDlg(msgs, ct, sWndID);
        m_pCommFrame->OpenSubscribeDlg(sMsgID, ct, sWndID);
    }
    else if (MSGCMD_GetAllDealer == sRecv)
    {
        m_pCommFrame->GetAllDealer(sMsgID);
    }
    else if (MSGCMD_MM_SendQuotRetV2 == sRecv)
    {
        Contact ct;
        SetString Success;
        SetString Failed;
        CMsgMng::Instance()->ParseMsg_SendQuotRetV2(msgs, ct, Success, Failed);
        m_pCommFrame->SendMMQuotRetV2(sMsgID, ct, Success, Failed);
    }
    else if (MSGCMD_BOND_SendQuotByCodeRetV2 == sRecv)
    {
        Contact ct;
        SetString Success;
        SetString Failed;
        CMsgMng::Instance()->ParseMsg_SendBondQuotByCodeRetV2(msgs, ct, Success, Failed);
        m_pCommFrame->SendBondQuotByCodeRetV2(sMsgID, ct, Success, Failed);
    }
    else if (MSGCMD_QMErrInfo == sRecv)
    {
        int nCode(0);
        string sInfo("");
        CMsgMng::Instance()->ParseMsg_QMErrInfo(msgs, nCode, sInfo);
        m_pCommFrame->QMErrInfo(nCode, sInfo);
    }
    else if (MSGCMD_Purchase_QueryStatus == sRecv)
    {
        LstPurchaseUkey lstQuery;
        CMsgMng::Instance()->ParseMsg_QueryPurchaseStatus(msgs, lstQuery);
        m_pCommFrame->QueryPurchaseStatus(sMsgID, lstQuery);
    }
    else if (MSGCMD_Purchase_Confirm == sRecv)
    {
        string sPurchaseID("");
        time_t tModifyTime(0);
        CMsgMng::Instance()->ParseMsg_ConfirmPurchase(msgs, sPurchaseID, tModifyTime);
        m_pCommFrame->ConfirmPurchase(sMsgID, sPurchaseID, tModifyTime);
    }
    else if (MSGCMD_Purchase_IsEditValid == sRecv)
    {
        Contact ct;
        CMsgMng::Instance()->ParseMsg_IsEditPurchaseValid(msgs, ct);
        m_pCommFrame->IsEditPurchaseValid(sMsgID, ct);
    }
    else if (MSGCMD_Purchase_Edit == sRecv)
    {
        Contact ct;
        std::string sWndID("");
        CMsgMng::Instance()->ParseMsg_EditPurchase(msgs, ct, sWndID);
        m_pCommFrame->EditPurchase(sMsgID, ct, sWndID);
    }
    else if (MSGCMD_MM_QueryQuotStatus == sRecv)
    {
        LstQuotStatusReq lstReq;
        CMsgMng::Instance()->ParseMsg_QueryMMQuotStatus(msgs, lstReq);
        m_pCommFrame->QueryMMQuotStatus(sMsgID, lstReq);
    }
    else if (MSGCMD_Bond_QueryQuotByCodeStatus == sRecv)
    {
        LstQuotStatusReq lstReq;
        CMsgMng::Instance()->ParseMsg_QueryBondQuotByCodeStatus(msgs, lstReq);
        m_pCommFrame->QueryBondQuotByCodeStatus(sMsgID, lstReq);
    }
    else if (MSGCMD_ContactInfoPush == sRecv)
    {
        LstPersonInfo lstPerson;
        LstGroupInfo lstGrp;
        CMsgMng::Instance()->ParseMsg_ContactInfoPush(msgs, lstPerson, lstGrp);
        m_pCommFrame->ContactInfoPush(sMsgID, lstPerson, lstGrp);
    }
    else if (MSGCMD_ContactStatusPush == sRecv)
    {
        LstContactStatus lstStatus;
        CMsgMng::Instance()->ParseMsg_ContactStatusPush(msgs, lstStatus);
        m_pCommFrame->ContactStatusPush(sMsgID, lstStatus);
    }
    else if (MSGCMD_QueryQMIDByQBAccountRet == sRecv)
    {
        LstQBAccount2QMID lstInfo;
        CMsgMng::Instance()->ParseMsg_QueryQMIDByQBAccountRet(msgs, lstInfo);
        m_pCommFrame->QueryQMIDByQBAccountRet(sMsgID, lstInfo);
    }
    else if (MSGCMD_Remind_QueryQMRemindConfigRet == sRecv)
    {
        QMRemindConfig RCfg;
        CMsgMng::Instance()->ParseQueryQMRemindConfigRet(msgs, RCfg);
        m_pCommFrame->QueryQMRemindConfigRet(sMsgID, RCfg);
    }
    else if (MSGCMD_Remind_AddQMRemindKeyItemRet == sRecv)
    {
        std::string sItemID("");
        CMsgMng::Instance()->ParseMsg_AddQMRemindKeyItemRet(msgs, sItemID);
        m_pCommFrame->AddQMRemindKeyItemRet(sMsgID, sItemID);
    }
    else if (MSGCMD_Remind_ModifyQMRemindKeyItemRet == sRecv)
    {
        bool bSuccess(false);
        CMsgMng::Instance()->ParseMsg_ModifyQMRemindKeyItemRet(msgs, bSuccess);
        m_pCommFrame->ModifyQMRemindKeyItemRet(sMsgID, bSuccess);
    }
    else if (MSGCMD_Remind_DelQMRemindKeyItemRet == sRecv)
    {
        SetString setSuccessIDs;
        CMsgMng::Instance()->ParseMsg_DelQMRemindKeyItemRet(msgs, setSuccessIDs);
        m_pCommFrame->DelQMRemindKeyItemRet(sMsgID, setSuccessIDs);
    }
    else if (MSGCMD_Remind_AddQMRemindFriendItemRet == sRecv)
    {
        std::string sItemID("");
        CMsgMng::Instance()->ParseMsg_AddQMRemindFriendItemRet(msgs, sItemID);
        m_pCommFrame->AddQMRemindFriendItemRet(sMsgID, sItemID);
    }
    else if (MSGCMD_Remind_ModifyQMRemindFriendItemRet == sRecv)
    {
        bool bSuccess(false);
        CMsgMng::Instance()->ParseMsg_ModifyQMRemindFriendItemRet(msgs, bSuccess);
        m_pCommFrame->ModifyQMRemindFriendItemRet(sMsgID, bSuccess);
    }
    else if (MSGCMD_Remind_DelQMRemindFriendItemRet == sRecv)
    {
        SetString setSuccessIDs;
        CMsgMng::Instance()->ParseMsg_DelQMRemindFriendItemRet(msgs, setSuccessIDs);
        m_pCommFrame->DelQMRemindFriendItemRet(sMsgID, setSuccessIDs);
    }
    else if (MSGCMD_Remind_SetQMRemindTypeRet == sRecv)
    {
        bool bSuccess(false);
        CMsgMng::Instance()->ParseMsg_SetQMRemindTypeRet(msgs, bSuccess);
        m_pCommFrame->SetQMRemindTypeRet(sMsgID, bSuccess);
    }
    else if (MSGCMD_ReqJoinGroupRet == sRecv)
    {
        bool bSuccess(false);
        CMsgMng::Instance()->ParseJoinGroupRet(msgs, bSuccess);
        m_pCommFrame->ReqJoinGroupRet(sMsgID, bSuccess);
    }
    else if (MSGCMD_ReqOpenNewsDetail == sRecv)
    {
        bool bDisablePDF(false);
        NewsInfoV2 info;
        CMsgMng::Instance()->ParseReqOpenNewsDetail(msgs, info, bDisablePDF);
        m_pCommFrame->ReqOpenNewsDetail(info, bDisablePDF);
    }
    else if (MSGCMD_ReqOpenNewsWnd == sRecv)
    {
        m_pCommFrame->ReqOpenNewsWnd();
    }
    else if (MSGCMD_QueryQMStatusRet == sRecv)
    {
        int status(0);
        CMsgMng::Instance()->ParseQueryQMStatusRet(msgs, status);
        m_pCommFrame->QueryQMStatusRet(status);
    }
    else if (MSGCMD_ModifyNoInstituteContactRet == sRecv)
    {
        LstNoInstituteContact contacts;
        CMsgMng::Instance()->ParseModifyNoInstituteContactRet(msgs, contacts);
        m_pCommFrame->ModifyNoInstituteContactRet(contacts);
    }
}