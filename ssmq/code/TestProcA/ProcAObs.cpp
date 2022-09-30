#include "stdafx.h"
#include "ProcAObs.h"


CProcAStatus::CProcAStatus(void)
{
}

CProcAStatus::~CProcAStatus(void)
{
}

sdbus::string CProcAStatus::GetBindKey()
{
	return "qm.status.qb";
}

SSMQ::SSMQDeliverType CProcAStatus::GetDeliverType()
{
	return SSMQ::SSMQDT_Publish;
}

void CProcAStatus::OnMsgs(const sdbus::Message& msgs)
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
        std::cout << sSessionID << std::endl;
	}
}