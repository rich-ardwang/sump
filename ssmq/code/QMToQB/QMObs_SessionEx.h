#pragma once
#include "QMObs_Session.h"

class CQMObs_Session6 : public CQMObs_Session
{
public:
	CQMObs_Session6(void);
	~CQMObs_Session6(void);

	sdbus::string GetBindKey();
};

class CQMObs_Session7 : public CQMObs_Session
{
public:
    CQMObs_Session7(void);
    ~CQMObs_Session7(void);

    sdbus::string GetBindKey();
};