#pragma once
#include "QBObs_Session.h"

class CQBObs_Session3 : public CQBObs_Session
{
public:
	CQBObs_Session3(void);
	~CQBObs_Session3(void);

	sdbus::string GetBindKey();
};
