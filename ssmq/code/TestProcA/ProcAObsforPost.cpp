#include "stdafx.h"
#include "ProcAObsforPost.h"


CProcAPost::CProcAPost(void)
{
}

CProcAPost::~CProcAPost(void)
{
}

sdbus::string CProcAPost::GetBindKey()
{
	return "qm.status";
}

SSMQ::SSMQDeliverType CProcAPost::GetDeliverType()
{
    return SSMQ::SSMQDT_Direct;
}

void CProcAPost::OnMsgs(const sdbus::Message& msgs)
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
	}
}