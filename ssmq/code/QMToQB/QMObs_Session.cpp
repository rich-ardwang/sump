#include "stdafx.h"
#include "QMObs_Session.h"
#include "MsgMng.h"

CQMObs_Session::CQMObs_Session(void)
{
}

CQMObs_Session::~CQMObs_Session(void)
{
}

sdbus::string CQMObs_Session::GetBindKey()
{
	string sRet = "qm.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

SSMQ::SSMQDeliverType CQMObs_Session::GetDeliverType()
{
	return SSMQ::SSMQDT_Direct;
}

void CQMObs_Session::OnMsgs(const sdbus::Message& msgs)
{
   	std::string sMsgID = msgs.GetMessageID();
	const sdbus::Message::Map& values = msgs.MapValue();
	sdbus::string sRecv;
	values.GetString(1, sRecv);
	if (MSGCMD_MM_SelContacts == sRecv)
	{
		std::string sSelID("");
        std::string sWndID("");
        int nLimit(200);
        bool bShowGrp(true);
		CMsgMng::Instance()->ParseMsg_SelContacts(msgs, sSelID, sWndID, nLimit, bShowGrp);
		m_pCommFrame->SelContacts(sSelID, sWndID, nLimit, bShowGrp);
	}
	else if (MSGCMD_MM_EditContacts == sRecv)
	{
		std::string sEditID("");
		LstContact lstCt;
        std::string sWndID("");
        int nLimit(200);
        bool bShowGrp(true);
		CMsgMng::Instance()->ParseMsg_EditContacts(msgs, sEditID, lstCt, sWndID, nLimit, bShowGrp);
		m_pCommFrame->EditContacts(sEditID, lstCt, sWndID, nLimit, bShowGrp);
	}
	else if (MSGCMD_MM_SendQuot == sRecv)
	{
		LstQuotSendInfo lstIn;
		CMsgMng::Instance()->ParseMsg_SendQuot(msgs, lstIn);
		m_pCommFrame->SendQuot(lstIn);
	}
	else if (MSGCMD_MM_QueryContactInfo == sRecv)
	{
		LstContact lstIn;
		CMsgMng::Instance()->ParseMsg_QueryContactsInfo(msgs, lstIn);
		m_pCommFrame->QueryContactInfo(sMsgID, lstIn);
	}
	else if (MSGCMD_RequestBondInfosRet == sRecv)
	{
		LstBondItem lstIn;
		CMsgMng::Instance()->ParseMsg_RequestBondInfosRet(msgs, lstIn);
		m_pCommFrame->RequestBondInfosRet(lstIn);
	}
	else if (MSGCMD_OpenMainPanel == sRecv)
	{
        std::string sWndID("");
        CMsgMng::Instance()->ParseMsg_OpenMainPanel(msgs, sWndID);
		m_pCommFrame->OpenMainPanel(sWndID);
	}
	else if (MSGCMD_RefreshBondInfos  == sRecv)
	{
		m_pCommFrame->RefreshBondInfos();
	}
	else if (MSGCMD_OpenDialogue  == sRecv)
	{
		Contact ct;
		std::string sMsgSend("");
        std::string sWndID("");
        bool bOpenDialog(true);
		CMsgMng::Instance()->ParseMsg_OpenDialogue(msgs, ct, sMsgSend, sWndID, bOpenDialog);
		m_pCommFrame->OpenDialogue(ct, sMsgSend, sWndID, bOpenDialog);
	}
	else if (MSGCMD_BOND_SendQuotByCode  == sRecv)
	{
		LstBondQuotByCodeSendInfo lstIn;
		CMsgMng::Instance()->ParseMsg_SendBondQuotByCode(msgs, lstIn);
		m_pCommFrame->SendBondQuotByCode(lstIn);
	}
	else if (MSGCMD_BOND_SendQuotByCondition  == sRecv)
	{
		LstBondQuotByConditionSendInfo lstIn;
		CMsgMng::Instance()->ParseMsg_SendBondQuotByCondition(msgs, lstIn);
		m_pCommFrame->SendBondQuotByCondition(lstIn);
	}
	else if (MSGCMD_MM_ReplyQuot  == sRecv)
	{
		QuotReplyInfo ItemIn;
        std::string sWndID("");
		CMsgMng::Instance()->ParseMsg_ReplyQuot(msgs, ItemIn, sWndID);
		m_pCommFrame->ReplyQuot(ItemIn, sWndID);
	}
	else if (MSGCMD_QueryCurUsrID  == sRecv)
	{
		m_pCommFrame->QueryCurUsrID();
	}
	else if (MSGCMD_QueryCurUsrFriendIDs == sRecv)
	{
		m_pCommFrame->QueryCurUsrFriendIDs();
	}
	else if (MSGCMD_QueryCurUsrGroupIDs == sRecv)
	{
		m_pCommFrame->QueryCurUsrGroupIDs();
	}
	else if (MSGCMD_SendMsg == sRecv)
	{
		LstContact lstCt;
		std::string sMsg("");
        std::string sWndID("");
		CMsgMng::Instance()->ParseMsg_SendMsg(msgs, lstCt, sMsg, sWndID);
		m_pCommFrame->SendMsg(lstCt, sMsg, sWndID);
	}
	else if (MSGCMD_QueryQMWndPos == sRecv)
	{
		std::string sWndID;
		CMsgMng::Instance()->ParseMsg_QueryQMWndPos(msgs, sWndID);
		m_pCommFrame->QueryQMWndPos(sMsgID, sWndID);
	}
    else if (MSGCMD_QueryQMWndHandle == sRecv)
    {
        std::string sWndID;
        CMsgMng::Instance()->ParseMsg_QueryQMWndHandle(msgs, sWndID);
        m_pCommFrame->QueryQMWndHandle(sMsgID, sWndID);
    }
    else if (MSGCMD_QuotShare_QueryShareGroup == sRecv)
    {
        m_pCommFrame->QuerySharedGroup(sMsgID);
    }
    else if (MSGCMD_SearchQMContact == sRecv)
    {
        ContactSearch si;
        CMsgMng::Instance()->ParseMsg_SearchQMContact(msgs, si);
        m_pCommFrame->SearchQMContact(sMsgID, si);
    }
    else if (MSGCMD_MM_SendQuotV2 == sRecv)
    {
        LstMMQuotSendInfoV2 lstIn;
        CMsgMng::Instance()->ParseMsg_SendQuotV2(msgs, lstIn);
        m_pCommFrame->SendQuotV2(sMsgID, lstIn);
    }
    else if (MSGCMD_BOND_SendQuotByCodeV2 == sRecv)
    {
        LstBondQuotByCodeSendInfoV2 lstIn;
        CMsgMng::Instance()->ParseMsg_SendBondQuotByCodeV2(msgs, lstIn);
        m_pCommFrame->SendBondQuotByCodeV2(sMsgID, lstIn);
    }
    else if (MSGCMD_BOND_ReplyQuotByCode == sRecv)
    {
        BondQuotByCodeReplyInfo ItemIn;
        std::string sWndID("");
        CMsgMng::Instance()->ParseMsg_ReplyBondQuotByCode(msgs, ItemIn, sWndID);
        m_pCommFrame->ReplyBondQuotByCode(ItemIn, sWndID);
    }
    else if (MSGCMD_Purchase_Send == sRecv)
    {
        Contact Ct;
        PurchaseInfo PInfo;
        std::string sWndID("");
        CMsgMng::Instance()->ParseMsg_Purchase(msgs, Ct, PInfo, sWndID);
        m_pCommFrame->Purchase(sMsgID, Ct, PInfo, sWndID);
    }
    else if (MSGCMD_ContactIsDealerRet == sRecv)
    {
        bool bRet;
        CMsgMng::Instance()->ParseMsg_ContactIsDealerRet(msgs, bRet);
        m_pCommFrame->ContactIsDealerRet(sMsgID, bRet);
    }
    else if (MSGCMD_GetAllDealerRet == sRecv)
    {
        SetUInt64 setID;
        CMsgMng::Instance()->ParseMsg_GetAllDealerRet(msgs, setID);
        m_pCommFrame->GetAllDealerRet(sMsgID, setID);
    }
    else if (MSGCMD_Purchase_QueryStatusRet == sRecv)
    {
        MapPurchaseStatus mapPStatus;
        CMsgMng::Instance()->ParseMsg_QueryPurchaseStatusRet(msgs, mapPStatus);
        m_pCommFrame->QueryPurchaseStatusRet(sMsgID, mapPStatus);
    }
    else if (MSGCMD_Purchase_ConfirmRet == sRecv)
    {
        int nStatus(0);
        CMsgMng::Instance()->ParseMsg_ConfirmPurchaseRet(msgs, nStatus);
        m_pCommFrame->ConfirmPurchaseRet(sMsgID, nStatus);
    }
    else if (MSGCMD_Purchase_IsEditValidRet == sRecv)
    {
        bool bRet;
        CMsgMng::Instance()->ParseMsg_IsEditPurchaseValidRet(msgs, bRet);
        m_pCommFrame->IsEditPurchaseValidRet(sMsgID, bRet);
    }
    else if (MSGCMD_Purchase_StatusPush == sRecv)
    {
        MapPurchaseStatus mapPStatus;
        CMsgMng::Instance()->ParseMsg_PurchaseStatusPush(msgs, mapPStatus);
        m_pCommFrame->PurchaseStatusPush(sMsgID, mapPStatus);
    }
    else if (MSGCMD_MM_QueryQuotStatusRet == sRecv)
    {
        LstQuotStatus lstRet;
        CMsgMng::Instance()->ParseMsg_QueryMMQuotStatusRet(msgs, lstRet);
        m_pCommFrame->QueryMMQuotStatusRet(sMsgID, lstRet);
    }
    else if (MSGCMD_Bond_QueryQuotByCodeStatusRet == sRecv)
    {
        LstQuotStatus lstRet;
        CMsgMng::Instance()->ParseMsg_QueryBondQuotByCodeStatusRet(msgs, lstRet);
        m_pCommFrame->QueryBondQuotByCodeStatusRet(sMsgID, lstRet);
    }
    else if (MSGCMD_Purchase_IsEditValidPush == sRecv)
    {
        Contact ct;
        bool bRet(false);
        CMsgMng::Instance()->ParseMsg_IsEditPurchaseValidPush(msgs, ct, bRet);
        m_pCommFrame->IsEditPurchaseValidPush(sMsgID, ct, bRet);
    }
    else if (MSGCMD_ContactIsDealerPush == sRecv)
    {
        Contact ct;
        bool bRet(false);
        CMsgMng::Instance()->ParseMsg_ContactIsDealerPush(msgs, ct, bRet);
        m_pCommFrame->ContactIsDealerPush(sMsgID, ct, bRet);
    }
    else if (MSGCMD_QueryQMIDByQBAccount == sRecv)
    {
        SetString QBAccounts;
        CMsgMng::Instance()->ParseMsg_QueryQMIDByQBAccount(msgs, QBAccounts);
        m_pCommFrame->QueryQMIDByQBAccount(sMsgID, QBAccounts);
    }
    else if (MSGCMD_Remind_QueryQMRemindConfig == sRecv)
    {
        m_pCommFrame->QueryQMRemindConfig(sMsgID);
    }
    else if (MSGCMD_Remind_AddQMRemindKeyItem == sRecv)
    {
        QMRemindKeyItem Item;
        CMsgMng::Instance()->ParseMsg_AddQMRemindKeyItem(msgs, Item);
        m_pCommFrame->AddQMRemindKeyItem(sMsgID, Item);
    }
    else if (MSGCMD_Remind_ModifyQMRemindKeyItem == sRecv)
    {
        QMRemindKeyItem Item;
        CMsgMng::Instance()->ParseMsg_ModifyQMRemindKeyItem(msgs, Item);
        m_pCommFrame->ModifyQMRemindKeyItem(sMsgID, Item);
    }
    else if (MSGCMD_Remind_DelQMRemindKeyItem == sRecv)
    {
        SetString setID;
        CMsgMng::Instance()->ParseMsg_DelQMRemindKeyItem(msgs, setID);
        m_pCommFrame->DelQMRemindKeyItem(sMsgID, setID);
    }
    else if (MSGCMD_Remind_AddQMRemindFriendItem == sRecv)
    {
        QMRemindFriendItem Item;
        CMsgMng::Instance()->ParseMsg_AddQMRemindFriendItem(msgs, Item);
        m_pCommFrame->AddQMRemindFriendItem(sMsgID, Item);
    }
    else if (MSGCMD_Remind_ModifyQMRemindFriendItem == sRecv)
    {
        QMRemindFriendItem Item;
        CMsgMng::Instance()->ParseMsg_ModifyQMRemindFriendItem(msgs, Item);
        m_pCommFrame->ModifyQMRemindFriendItem(sMsgID, Item);
    }
    else if (MSGCMD_Remind_DelQMRemindFriendItem == sRecv)
    {
        SetString setID;
        CMsgMng::Instance()->ParseMsg_DelQMRemindFriendItem(msgs, setID);
        m_pCommFrame->DelQMRemindFriendItem(sMsgID, setID);
    }
    else if (MSGCMD_Remind_SetQMRemindType == sRecv)
    {
        unsigned int nRemindType(0);
        CMsgMng::Instance()->ParseMsg_SetQMRemindType(msgs, nRemindType);
        m_pCommFrame->SetQMRemindType(sMsgID, nRemindType);
    }
    else if (MSGCMD_OpenNewsBulletin == sRecv)
    {
        string sWndID("");
        CMsgMng::Instance()->ParseMsg_OpenNewsBulletin(msgs, sWndID);
        m_pCommFrame->OpenNewsBulletin(sWndID);
    }
    else if (MSGCMD_QueryCustomService == sRecv)
    {
        m_pCommFrame->QueryCustomService(sMsgID);
    }
    else if (MSGCMD_QBModifyPwd == sRecv)
    {
        string sNewPwd("");
        CMsgMng::Instance()->ParseMsg_QBModifyPwd(msgs, sNewPwd);
        m_pCommFrame->QBModifyPwd(sNewPwd);
    }
    else if (MSGCMD_ShareBond == sRecv)
    {
        LstContact lstCt;
        LstBondShare lstBS;
        std::string sWndID("");
        CMsgMng::Instance()->ParseMsg_ShareBond(msgs, lstCt, lstBS, sWndID);
        m_pCommFrame->ShareBond(lstCt, lstBS, sWndID);
    }
    else if (MSGCMD_ReqJoinGroup == sRecv)
    {
        unsigned long long llGroup;
        CMsgMng::Instance()->ParseReqJoinGroup(msgs, llGroup);
        m_pCommFrame->ReqJoinGroup(sMsgID, llGroup);
    }
	else if (MSGCMD_ReqOpenNews == sRecv)
	{
		NewsInfo info;
		CMsgMng::Instance()->ParseReqOpenNews(msgs, info);
		m_pCommFrame->ReqOpenNews(info);
	}
    else if (MSGCMD_SharePic == sRecv)
    {
        PicInfo info;
        LstContact lstCt;
        CMsgMng::Instance()->ParseSharePic(msgs, info, lstCt);
        m_pCommFrame->SharePic(info, lstCt);
    }
    else if (MSGCMD_ShareNewsDetail == sRecv)
    {
        NewsInfoV2 info;
        LstContact lstCt;
        bool bDisablePDF(false);
        CMsgMng::Instance()->ParseShareNewsDetail(msgs, info, lstCt, bDisablePDF);
        m_pCommFrame->ShareNewsDetail(info, lstCt, bDisablePDF);
    }
    else if (MSGCMD_OpenDialogueByUserID == sRecv)
    {
        std::string userID;
        CMsgMng::Instance()->ParseOpenDialogueByUserID(msgs, userID);
        m_pCommFrame->OpenDialogueByUserID(userID);
    }
    else if (MSGCMD_QueryQMStatus == sRecv)
    {
        m_pCommFrame->QueryQMStatus();
    }
    else if (MSGCMD_ReqOpenSellsWnd == sRecv)
    {
        string sWndID;
        CMsgMng::Instance()->ParseReqOpenSellsWnd(msgs, sWndID);
        m_pCommFrame->ReqOpenSellsWnd(sWndID);
    }
    else if (MSGCMD_ReqQueryNoInstituteContact == sRecv)
    {
        m_pCommFrame->ReqQueryNoInstituteContact(sMsgID);      
    }
    else if (MSGCMD_ReqSetNoInstituteContact == sRecv)
    {
        std::list<std::string> QBAccounts;
        CMsgMng::Instance()->ParseReqSetNoInstituteContact(msgs, QBAccounts);
        m_pCommFrame->ReqSetNoInstituteContact(QBAccounts);
    }
    else if (MSGCMD_ReqOpenSellsSearchWnd == sRecv)
    {
        string sWndID;
        CMsgMng::Instance()->ParseReqOpenSellsSearchWnd(msgs, sWndID);
        m_pCommFrame->ReqOpenSellsSearchWnd(sWndID);
    }
    else if (MSGCMD_OpenDialogueFromAd == sRecv)
    {
        std::string userID;
        CMsgMng::Instance()->ParseReqOpenDialogueFromAd(msgs, userID);
        m_pCommFrame->ReqOpenDialogueFromAd(userID);
    }
}