#include "StdAfx.h"
#include "MQCallback.h"
#include "sdbus\connection.h"
#include <assert.h>


CMQCallback::CMQCallback(void)
:m_bStopDispatch(true)
,m_pDispatchThread(NULL)
{
	BeginDispatchThread();
}

CMQCallback::~CMQCallback(void)
{
	EndDispatchThread();
}

void CMQCallback::InitBySSMQObs(SSMQ::ISSMQObs* pObs)
{
	if (!pObs)
	{
		assert(FALSE);
		return;
	}
	
	Init(pObs->GetBindKey(), TransDeliverType(pObs->GetDeliverType()));
	m_pRelObs = pObs;
}

void CMQCallback::InitByCedaATSMQObs(SSMQ::ISSMQObs* pObs, const sdbus::string &cedaHeader)
{
    if (!pObs) {
        assert(FALSE);
        return;
    }

    Init(GetCedaATSMQMngIns()->getCedaBindKey(cedaHeader, pObs->GetBindKey()), TransDeliverType(pObs->GetDeliverType()));
    m_pRelObs = pObs;
}

void CMQCallback::OnMsgs(const sdbus::Message ** msgs , int count)
{
	if (!m_pRelObs)
	{
		assert(FALSE);
		return;
	}
	if (!(*msgs))
	{
		return;
	}

	const sdbus::Message::Map& values = (*msgs)->MapValue();
	if (values.Size() < 1)
	{
		//过滤空的消息
		return;
	}

	AddMsgToQueue(**msgs);
	//m_pRelObs->OnMsgs(**msgs);
}

int CMQCallback::TransDeliverType(SSMQ::SSMQDeliverType enType)
{
	switch (enType)
	{
	case SSMQ::SSMQDT_Direct:
		return sdbus::DeliverDirect;
	case SSMQ::SSMQDT_Publish:
		return sdbus::DeliverPublish;
	default:
		return sdbus::DeliverDirect;
	}
}

void CMQCallback::AddMsgToQueue(const sdbus::Message& msg)
{
	boost::mutex::scoped_lock lock(m_mutex);
	m_MsgQueue.push(msg);
    lock.unlock();
	m_conditionRecv.notify_one();
}

bool CMQCallback::GetMsgFromQueue(sdbus::Message& msg)
{
    boost::mutex::scoped_lock lock(m_mutex);
    //m_conditionRecv.wait(lock);
    //if (m_MsgQueue.empty())
    //{
    //    return false;
    //}
    while (m_MsgQueue.empty())
    {
        if (m_bStopDispatch)
        {
            return false;
        }
        m_conditionRecv.wait(lock);
    }
    msg = m_MsgQueue.front();
    m_MsgQueue.pop();
    return true;
}

void CMQCallback::DispatchMsg()
{
	sdbus::Message WorkMsg;

	while (!m_bStopDispatch)
	{
		if (!GetMsgFromQueue(WorkMsg))
		{
			continue;
		}

		m_pRelObs->OnMsgs(WorkMsg);
	}
}

void CMQCallback::BeginDispatchThread()
{
	m_bStopDispatch = false;
	if (m_pDispatchThread)
	{
		return;
	}
	m_pDispatchThread = new boost::thread(boost::bind(&CMQCallback::DispatchMsg, this));
}

void CMQCallback::EndDispatchThread()
{
	if (!m_pDispatchThread)
	{
		return;
	}

	m_bStopDispatch = true;
	m_conditionRecv.notify_one();
	m_pDispatchThread->join();
	delete m_pDispatchThread;
	m_pDispatchThread = NULL;
}