#pragma once
#include "SSMQ\ISSMQObs.h"



class CProcAPost : public SSMQ::ISSMQObs
{
public:
	CProcAPost(void);
	~CProcAPost(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
