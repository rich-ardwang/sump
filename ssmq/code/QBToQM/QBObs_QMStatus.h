#pragma once
#include "QBObs_Base.h"

class CQBObs_QMStatus : public CQBObs_Base
{
public:
	CQBObs_QMStatus(void);
	~CQBObs_QMStatus(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
