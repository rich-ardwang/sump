#include "stdafx.h"
#include "QBObs_Session3.h"

CQBObs_Session3::CQBObs_Session3(void)
{
}

CQBObs_Session3::~CQBObs_Session3(void)
{
}

sdbus::string CQBObs_Session3::GetBindKey()
{
	string sRet = "qb3.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

