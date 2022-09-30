#include "stdafx.h"
#include "QBObs_Session6.h"

CQBObs_Session6::CQBObs_Session6(void)
{
}

CQBObs_Session6::~CQBObs_Session6(void)
{
}

sdbus::string CQBObs_Session6::GetBindKey()
{
	string sRet = "qb6.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

