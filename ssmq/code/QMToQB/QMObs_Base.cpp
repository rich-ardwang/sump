#include "stdafx.h"
#include "QMObs_Base.h"

CQMObs_Base::CQMObs_Base(void)
:m_pCommFrame(NULL)
{
}

CQMObs_Base::~CQMObs_Base(void)
{
}

sdbus::string CQMObs_Base::GetBindKey()
{
	return "";
}

SSMQ::SSMQDeliverType CQMObs_Base::GetDeliverType()
{
	return SSMQ::SSMQDT_Undefine;
}

void CQMObs_Base::OnMsgs(const sdbus::Message& msgs)
{

}

void CQMObs_Base::SetCommFrame(IQMCommFrame* pCommFrame)
{
	m_pCommFrame = pCommFrame;
}
