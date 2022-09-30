#pragma once
#include "QMObs_Base.h"

class CQMObs_Mng : public CQMObs_Base
{
public:
	CQMObs_Mng(void);
	~CQMObs_Mng(void);

	sdbus::string GetBindKey();
	SSMQ::SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);
};
