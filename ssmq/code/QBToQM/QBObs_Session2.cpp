#include "stdafx.h"
#include "QBObs_Session2.h"

CQBObs_Session2::CQBObs_Session2(void)
{
}

CQBObs_Session2::~CQBObs_Session2(void)
{
}

sdbus::string CQBObs_Session2::GetBindKey()
{
	string sRet = "qb2.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}
