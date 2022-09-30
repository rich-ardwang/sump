#include "stdafx.h"
#include "QBObs_QMStatus.h"

CQBObs_QMStatus::CQBObs_QMStatus(void)
{
}

CQBObs_QMStatus::~CQBObs_QMStatus(void)
{
}

sdbus::string CQBObs_QMStatus::GetBindKey()
{
	return "qm.status";
}

SSMQ::SSMQDeliverType CQBObs_QMStatus::GetDeliverType()
{
	return SSMQ::SSMQDT_Publish;
}

void CQBObs_QMStatus::OnMsgs(const sdbus::Message& msgs)
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
		m_pCommFrame->SetQMStatus(nStatus, sSessionID.c_str());
	}
}