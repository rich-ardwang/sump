#include "stdafx.h"
#include "QMObs_Session2.h"

CQMObs_Session2::CQMObs_Session2(void)
{
}

CQMObs_Session2::~CQMObs_Session2(void)
{
}

sdbus::string CQMObs_Session2::GetBindKey()
{
	string sRet = "qm2.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

