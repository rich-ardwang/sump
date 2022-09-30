#include "stdafx.h"
#include "QMObs_SessionEx.h"

///////////////////////class CQMObs_Session6///////////////////////

CQMObs_Session6::CQMObs_Session6(void)
{
}

CQMObs_Session6::~CQMObs_Session6(void)
{
}

sdbus::string CQMObs_Session6::GetBindKey()
{
	string sRet = "qm6.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}

///////////////////////class CQMObs_Session7///////////////////////

CQMObs_Session7::CQMObs_Session7(void)
{
}

CQMObs_Session7::~CQMObs_Session7(void)
{
}

sdbus::string CQMObs_Session7::GetBindKey()
{
    string sRet = "qm7.";
    sRet += m_pCommFrame->GetSessionID();
    return sRet;
}