#pragma once
#include "ISSMQObs.h"
#include "IQBCommFrame.h"

class CQBObs_Base : public SSMQ::ISSMQObs
{
public:
	CQBObs_Base(void);
	virtual ~CQBObs_Base(void);

	//Interface of SSMQ::ISSMQObs
	virtual sdbus::string GetBindKey();
	virtual SSMQ::SSMQDeliverType GetDeliverType();
	virtual void OnMsgs(const sdbus::Message& msgs);

	void SetCommFrame(IQBCommFrame* pCommFrame);

protected:
	IQBCommFrame* m_pCommFrame;
};
