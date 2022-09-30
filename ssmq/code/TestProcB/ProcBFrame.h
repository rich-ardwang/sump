#pragma once
#include "SSMQ\ISSMQMng.h"
#include <string>
#include <iostream>

using namespace std;


class CProcBFrame : public SSMQ::ISSMQMngCallback
{
public:
	CProcBFrame(void);
	virtual ~CProcBFrame(void);

	//Interface of ISSMQMngCallback
	void CloseClient();
/*
	//等待心跳超时
	bool WaitforHeartbeat();
	//心跳超时处理
	void HBTimeout();

protected:
	void InitObs();
	void ClearObs();
	void StartQM();
	bool WaitForRightStatus();
	std::string GetSendQueueID(const sdbus::Message& msgs);
	void StartGuardThread();
	void StopGuardThread();

	//Heartbeat
	void Heartbeat();
	void HeartbeatThread();
	void StartHeartbeat();
	void StopHeartbeat();

public:
	volatile bool m_bStopGuard;
	HANDLE m_hHeartbeatEvent;

private:
    HANDLE m_HeartbeatThread;
	boost::thread* m_ThreadHeartbeat;
	bool m_bStopHeartbeat;
*/
};
