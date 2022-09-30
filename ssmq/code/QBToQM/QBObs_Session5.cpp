#include "stdafx.h"
#include "QBObs_Session5.h"

CQBObs_Session5::CQBObs_Session5(void)
{
}

CQBObs_Session5::~CQBObs_Session5(void)
{
}

sdbus::string CQBObs_Session5::GetBindKey()
{
	string sRet = "qb5.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

