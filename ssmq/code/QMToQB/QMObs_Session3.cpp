#include "stdafx.h"
#include "QMObs_Session3.h"

CQMObs_Session3::CQMObs_Session3(void)
{
}

CQMObs_Session3::~CQMObs_Session3(void)
{
}

sdbus::string CQMObs_Session3::GetBindKey()
{
	string sRet = "qm3.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}
