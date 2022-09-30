#pragma once
#include "QBObs_Base.h"

class CQBObs_Session : public CQBObs_Base
{
public:
	CQBObs_Session(void);
	~CQBObs_Session(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
