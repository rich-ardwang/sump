#pragma once

#ifdef AUTH_DLL
#define _IMP_AUTH_EXP_CLASS _declspec(dllexport)
#else
#define _IMP_AUTH_EXP_CLASS _declspec(dllimport)
#endif

struct _IMP_AUTH_EXP_CLASS AuthMessage
{
public:
	const char* category;
	const char* subCategory;
	const char* sourceIds;

public:
	void release();
};