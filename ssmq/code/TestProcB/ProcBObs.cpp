#include "stdafx.h"
#include "ProcBObs.h"


CProcBStatus::CProcBStatus(void)
{
}

CProcBStatus::~CProcBStatus(void)
{
}

sdbus::string CProcBStatus::GetBindKey()
{
	return "qm.status";
}

SSMQ::SSMQDeliverType CProcBStatus::GetDeliverType()
{
	return SSMQ::SSMQDT_Publish;
}

void CProcBStatus::OnMsgs(const sdbus::Message& msgs)
{
	const sdbus::Message::Map& values = msgs.MapValue();
	sdbus::string sRecv;
	values.GetString(1, sRecv);
	if (sRecv == "heartbeat")
	{
		sdbus::string sSessionID("");
		int nStatus = 0;
		//values.GetInt32(2, nStatus);
		values.GetString(3, sSessionID);
        cout << "[Publish], " << sSessionID << endl;
	}
}