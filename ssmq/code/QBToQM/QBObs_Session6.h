#pragma once
#include "QBObs_Session.h"

class CQBObs_Session6 : public CQBObs_Session
{
public:
	CQBObs_Session6(void);
	~CQBObs_Session6(void);

	sdbus::string GetBindKey();
};
