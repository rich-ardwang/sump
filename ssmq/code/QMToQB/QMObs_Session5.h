#pragma once
#include "QMObs_Session.h"

class CQMObs_Session5 : public CQMObs_Session
{
public:
	CQMObs_Session5(void);
	~CQMObs_Session5(void);

	sdbus::string GetBindKey();
};
