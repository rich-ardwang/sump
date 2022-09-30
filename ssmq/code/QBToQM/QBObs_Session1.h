#pragma once
#include "QBObs_Session.h"

class CQBObs_Session1 : public CQBObs_Session
{
public:
	CQBObs_Session1(void);
	~CQBObs_Session1(void);

	sdbus::string GetBindKey();
};
