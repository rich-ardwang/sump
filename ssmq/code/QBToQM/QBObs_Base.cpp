#include "stdafx.h"
#include "QBObs_Base.h"

CQBObs_Base::CQBObs_Base(void)
:m_pCommFrame(NULL)
{
}

CQBObs_Base::~CQBObs_Base(void)
{
}

sdbus::string CQBObs_Base::GetBindKey()
{
	return "";
}

SSMQ::SSMQDeliverType CQBObs_Base::GetDeliverType()
{
	return SSMQ::SSMQDT_Undefine;
}

void CQBObs_Base::OnMsgs(const sdbus::Message& msgs)
{

}

void CQBObs_Base::SetCommFrame(IQBCommFrame* pCommFrame)
{
	m_pCommFrame = pCommFrame;
}
