#pragma once
#include "sdbus/callback.h"
#include "ISSMQObs.h"
#include "SSMQMng.h"
#include <queue>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

typedef std::queue<sdbus::Message> MsgQueue;

class CMQCallback : public sdbus::Callback
{
public:
	CMQCallback(void);
	virtual ~CMQCallback(void);

public:
	void InitBySSMQObs(SSMQ::ISSMQObs* pObs);
    void InitByCedaATSMQObs(SSMQ::ISSMQObs* pObs, const sdbus::string &cedaHeader);
	void OnMsgs(const sdbus::Message ** msgs , int count);

protected:
	int TransDeliverType(SSMQ::SSMQDeliverType enType);
	void AddMsgToQueue(const sdbus::Message& msg);
	bool GetMsgFromQueue(sdbus::Message& msg);
	void DispatchMsg();
	void BeginDispatchThread();
	void EndDispatchThread();

private:
	SSMQ::ISSMQObs* m_pRelObs;
	MsgQueue m_MsgQueue;
	boost::mutex m_mutex;
	boost::condition m_conditionRecv;
	volatile bool m_bStopDispatch;
	boost::thread* m_pDispatchThread;
};
