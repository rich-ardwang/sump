#pragma once

#ifdef GATEWAY_API_DLL
#define _IMP_GATEWAY_CONNECTOR_EXP_CLASS _declspec(dllexport)
#else
#define _IMP_GATEWAY_CONNECTOR_EXP_CLASS _declspec(dllimport)
#endif

#include <string>
#include <vector>

#include "callback.h"
#include "authority_message.h"

class _IMP_GATEWAY_CONNECTOR_EXP_CLASS GatewayConnector {
public:
	GatewayConnector();
	~GatewayConnector();

public:
	GatewayConnector(CallBack* cb);
	void setReconnectArgs(const int count, const int interval = 3);

public:
	bool connect(const char* ip, const int port, const int timeout = 5);
	bool qbLogin(const char* userName, const char* passWord, char** tokenResult, const int timeout = 5);
	void getAuthority(const char* token);
	bool syncRequest(const char* token, const int funcCode, const char* sdnBytes, const int timeout = 5);

public:
	void setCallBack(CallBack* cb);
	CallBack* getCallBack();

private:
	CallBack* m_cb;
};