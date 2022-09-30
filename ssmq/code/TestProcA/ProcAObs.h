#pragma once
#include "SSMQ\ISSMQObs.h"
#include <iostream>
#include <string>



class CProcAStatus : public SSMQ::ISSMQObs
{
public:
	CProcAStatus(void);
	~CProcAStatus(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
