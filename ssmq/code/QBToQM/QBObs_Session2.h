#pragma once
#include "QBObs_Session.h"

class CQBObs_Session2 : public CQBObs_Session
{
public:
	CQBObs_Session2(void);
	~CQBObs_Session2(void);

	sdbus::string GetBindKey();
};
