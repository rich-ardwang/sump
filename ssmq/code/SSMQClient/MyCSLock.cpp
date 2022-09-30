#include "StdAfx.h"
#include "MyCSLock.h"

CMyCSLock::CMyCSLock(CRITICAL_SECTION* pcs)
:m_pcs(pcs)
{
	EnterCriticalSection(m_pcs);
}

CMyCSLock::~CMyCSLock(void)
{
	LeaveCriticalSection(m_pcs);
}
