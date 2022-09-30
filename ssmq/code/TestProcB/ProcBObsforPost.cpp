#include "stdafx.h"
#include "ProcBObsforPost.h"


CProcBPost::CProcBPost(void)
{
}

CProcBPost::~CProcBPost(void)
{
}

sdbus::string CProcBPost::GetBindKey()
{
	return "qm.status2";
}

SSMQ::SSMQDeliverType CProcBPost::GetDeliverType()
{
	return SSMQ::SSMQDT_Direct;
}

void CProcBPost::OnMsgs(const sdbus::Message& msgs)
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
        cout << "[Post], " << sSessionID << endl;
	}
}