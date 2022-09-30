#include "stdafx.h"
#include "QBObs_Session1.h"

CQBObs_Session1::CQBObs_Session1(void)
{
}

CQBObs_Session1::~CQBObs_Session1(void)
{
}

sdbus::string CQBObs_Session1::GetBindKey()
{
	string sRet = "qb1.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

