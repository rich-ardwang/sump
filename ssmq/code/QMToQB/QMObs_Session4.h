#pragma once
#include "QMObs_Session.h"

class CQMObs_Session4 : public CQMObs_Session
{
public:
	CQMObs_Session4(void);
	~CQMObs_Session4(void);

	sdbus::string GetBindKey();
};
