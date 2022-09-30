#include "stdafx.h"
#include "QMObs_Mng.h"
#include "MsgMng.h"

CQMObs_Mng::CQMObs_Mng(void)
{
}

CQMObs_Mng::~CQMObs_Mng(void)
{
}

sdbus::string CQMObs_Mng::GetBindKey()
{
	return "qm.mng";
}

SSMQ::SSMQDeliverType CQMObs_Mng::GetDeliverType()
{
	return SSMQ::SSMQDT_Publish;
}

void CQMObs_Mng::OnMsgs(const sdbus::Message& msgs)
{
	const sdbus::Message::Map& values = msgs.MapValue();
	sdbus::string sRecv;
	values.GetString(1, sRecv);
	if (sRecv == MSGCMD_QuitQM)
	{
		bool bChkSession;
		std::string sSessionID;
		CMsgMng::Instance()->ParseMsg_QuitQM(msgs, bChkSession, sSessionID);
		m_pCommFrame->Quit(bChkSession, sSessionID);
	}
}