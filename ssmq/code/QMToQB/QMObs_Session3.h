#pragma once
#include "QMObs_Session.h"

class CQMObs_Session3 : public CQMObs_Session
{
public:
	CQMObs_Session3(void);
	~CQMObs_Session3(void);

	sdbus::string GetBindKey();
};
