#include "stdafx.h"
#include "QMObs_Session5.h"

CQMObs_Session5::CQMObs_Session5(void)
{
}

CQMObs_Session5::~CQMObs_Session5(void)
{
}

sdbus::string CQMObs_Session5::GetBindKey()
{
	string sRet = "qm5.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

