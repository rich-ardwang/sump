#pragma once
#include "QMObs_Session.h"

class CQMObs_Session1 : public CQMObs_Session
{
public:
	CQMObs_Session1(void);
	~CQMObs_Session1(void);

	sdbus::string GetBindKey();
};
