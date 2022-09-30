#include "stdafx.h"
#include "QBObs_SessionEx.h"

////////////////////////class CQBObs_Session7////////////////////////

CQBObs_Session7::CQBObs_Session7(void)
{
}

CQBObs_Session7::~CQBObs_Session7(void)
{
}

sdbus::string CQBObs_Session7::GetBindKey()
{
	string sRet = "qb7.";
	sRet += m_pCommFrame->GetSessionID();
	return sRet;
}


////////////////////////class CQBObs_Session8////////////////////////

CQBObs_Session8::CQBObs_Session8(void)
{
}

CQBObs_Session8::~CQBObs_Session8(void)
{
}

sdbus::string CQBObs_Session8::GetBindKey()
{
    string sRet = "qb8.";
    sRet += m_pCommFrame->GetSessionID();
    return sRet;
}

////////////////////////class CQBObs_Session9////////////////////////

CQBObs_Session9::CQBObs_Session9(void)
{
}

CQBObs_Session9::~CQBObs_Session9(void)
{
}

sdbus::string CQBObs_Session9::GetBindKey()
{
    string sRet = "qb9.";
    sRet += m_pCommFrame->GetSessionID();
    return sRet;
}
