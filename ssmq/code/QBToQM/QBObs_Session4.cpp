#include "stdafx.h"
#include "QBObs_Session4.h"

CQBObs_Session4::CQBObs_Session4(void)
{
}

CQBObs_Session4::~CQBObs_Session4(void)
{
}

sdbus::string CQBObs_Session4::GetBindKey()
{
	string sRet = "qb4.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

