#pragma once
#include "ISSMQObs.h"
#include "IQMCommFrame.h"

class CQMObs_Base : public SSMQ::ISSMQObs
{
public:
	CQMObs_Base(void);
	virtual ~CQMObs_Base(void);

	//Interface of SSMQ::ISSMQObs
	virtual sdbus::string GetBindKey();
	virtual SSMQ::SSMQDeliverType GetDeliverType();
	virtual void OnMsgs(const sdbus::Message& msgs);

	void SetCommFrame(IQMCommFrame* pCommFrame);

protected:
	IQMCommFrame* m_pCommFrame;
};
