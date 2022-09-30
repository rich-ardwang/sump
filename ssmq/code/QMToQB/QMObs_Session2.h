#pragma once
#include "QMObs_Session.h"

class CQMObs_Session2 : public CQMObs_Session
{
public:
	CQMObs_Session2(void);
	~CQMObs_Session2(void);

	sdbus::string GetBindKey();
};
