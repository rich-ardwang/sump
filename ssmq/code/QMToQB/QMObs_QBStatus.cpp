#include "stdafx.h"
#include "QMObs_QBStatus.h"

CQMObs_QBStatus::CQMObs_QBStatus(void)
{
}

CQMObs_QBStatus::~CQMObs_QBStatus(void)
{
}

sdbus::string CQMObs_QBStatus::GetBindKey()
{
	return "qb.status";
}

SSMQ::SSMQDeliverType CQMObs_QBStatus::GetDeliverType()
{
	return SSMQ::SSMQDT_Publish;
}

void CQMObs_QBStatus::OnMsgs(const sdbus::Message& msgs)
{
	const sdbus::Message::Map& values = msgs.MapValue();
	sdbus::string sRecv;
	values.GetString(1, sRecv);
	if (sRecv == "heartbeat")
	{
		sdbus::string sSessionID("");
		int nStatus = 0;
		values.GetInt32(2, nStatus);
		values.GetString(3, sSessionID);
		m_pCommFrame->SetQBStatus(nStatus, sSessionID.c_str());
	}
}