#pragma once
#include "QBObs_Session.h"

class CQBObs_Session5 : public CQBObs_Session
{
public:
	CQBObs_Session5(void);
	~CQBObs_Session5(void);

	sdbus::string GetBindKey();
};
