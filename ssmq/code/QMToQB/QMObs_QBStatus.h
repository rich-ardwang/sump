#pragma once
#include "QMObs_Base.h"

class CQMObs_QBStatus : public CQMObs_Base
{
public:
	CQMObs_QBStatus(void);
	~CQMObs_QBStatus(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
