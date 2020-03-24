#pragma once

#include "authority_message.h"

class CallBack {
public:
	virtual void onDisConnected() = 0;
	virtual void onKickOut() = 0;
	virtual void onAuthMessage(AuthMessage* pa, int len) = 0;
};