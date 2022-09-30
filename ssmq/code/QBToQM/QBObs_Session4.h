#pragma once
#include "QBObs_Session.h"

class CQBObs_Session4 : public CQBObs_Session
{
public:
	CQBObs_Session4(void);
	~CQBObs_Session4(void);

	sdbus::string GetBindKey();
};
