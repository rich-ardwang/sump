#pragma once
#include "QBObs_Session.h"

class CQBObs_Session7 : public CQBObs_Session
{
public:
	CQBObs_Session7(void);
	~CQBObs_Session7(void);

	sdbus::string GetBindKey();
};


class CQBObs_Session8 : public CQBObs_Session
{
public:
    CQBObs_Session8(void);
    ~CQBObs_Session8(void);

    sdbus::string GetBindKey();
};


class CQBObs_Session9 : public CQBObs_Session
{
public:
    CQBObs_Session9(void);
    ~CQBObs_Session9(void);

    sdbus::string GetBindKey();
};