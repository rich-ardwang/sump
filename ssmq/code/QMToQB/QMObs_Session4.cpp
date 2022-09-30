#include "stdafx.h"
#include "QMObs_Session4.h"

CQMObs_Session4::CQMObs_Session4(void)
{
}

CQMObs_Session4::~CQMObs_Session4(void)
{
}

sdbus::string CQMObs_Session4::GetBindKey()
{
	string sRet = "qm4.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

