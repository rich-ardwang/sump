#pragma once

class __declspec(dllexport) CMyCSLock
{
public:
	CMyCSLock(CRITICAL_SECTION* pcs);
	~CMyCSLock(void);

private:
	CRITICAL_SECTION* m_pcs;
};
