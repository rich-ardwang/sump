#pragma once
#include "QMObs_Base.h"

class CQMObs_Session : public CQMObs_Base
{
public:
	CQMObs_Session(void);
	~CQMObs_Session(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
