#pragma once
#include "SSMQ\ISSMQObs.h"
#include <iostream>

using namespace std;



class CProcBPost : public SSMQ::ISSMQObs
{
public:
	CProcBPost(void);
	~CProcBPost(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
