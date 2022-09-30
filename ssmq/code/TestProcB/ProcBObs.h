#pragma once
#include "SSMQ\ISSMQObs.h"
#include <iostream>

using namespace std;



class CProcBStatus : public SSMQ::ISSMQObs
{
public:
	CProcBStatus(void);
	~CProcBStatus(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
