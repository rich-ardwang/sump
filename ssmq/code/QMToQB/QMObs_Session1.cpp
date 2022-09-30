#include "stdafx.h"
#include "QMObs_Session1.h"

CQMObs_Session1::CQMObs_Session1(void)
{
}

CQMObs_Session1::~CQMObs_Session1(void)
{
}

sdbus::string CQMObs_Session1::GetBindKey()
{
	string sRet = "qm1.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

