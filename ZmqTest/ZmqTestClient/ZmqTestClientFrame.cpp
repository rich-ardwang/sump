/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  ZmqTestClientFrame.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/27
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#include "stdafx.h"
#include "ZmqTestClientFrame.h"
#include "..\ZmqTestServer\ZmqTestDefine.h"
#include <Psapi.h>
#include <process.h>
#include "sdbus/helper.h"
#include "ClientMsgQueue.h"

CZmqTestClientFrame::CZmqTestClientFrame()
	: m_sBrokerUrl("tcp://127.0.0.1:5554")
	, m_nMsgQueueSize(128)
	, m_hProcMutex(NULL)
	, m_bNeedReleaseMutex(false)
	, m_bStopGuard(false)
{
	m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, GetExitEventName().c_str());
}

CZmqTestClientFrame::~CZmqTestClientFrame()
{
	CloseHandle(m_hExitEvent);
	if (m_bNeedReleaseMutex)
	{
		ReleaseMutex(m_hProcMutex);
	}
	CloseHandle(m_hProcMutex);
}

void CZmqTestClientFrame::Run()
{
	if (!ProcessSingleton()) return;

	BeginListenMngMsg();
	StartGuardThread();
	PickMsgFromQueueAndSend();
	WaitForSingleObject(m_hExitEvent, INFINITE);
	StopGuardThread();
	m_conn.DelHandler(this);
}

void CZmqTestClientFrame::PickMsgFromQueueAndSend()
{
	sdbus::Message msg;
	sdbus::Message msgOut;
	CMsgQueueStruct unit;
	if (!GetClientMessageQueue().PopFrontReq(unit)) return;
	char buf[100];
	_i64toa_s(unit.m_nId, buf,_countof(buf), 10);
	msg.SetString(11, buf);
	LARGE_INTEGER litmp;
	QueryPerformanceCounter(&litmp);
	unit.m_nClientSendTime = GetHighTimeRes().GetPerformanceCounter();
	_i64toa_s(unit.m_nClientSendTime, buf, _countof(buf), 10);
	msg.SetString(12, buf);
	m_conn.Send(ZMQ_TOPIC_MNG, msg); //������Ϣ��
}

bool CZmqTestClientFrame::ReceiveAll()
{
	return GetClientMessageQueue().ReceiveAll();
}

t_string CZmqTestClientFrame::GetExitEventName()
{
	return "Sumscope_ZmqTestClient_Exit";
}

t_string CZmqTestClientFrame::GetProcessMutexName()
{
	return "Sumscope_ZmqTestClient_Frame";
}

void CZmqTestClientFrame::BeginListenMngMsg()
{
	boost::mutex::scoped_lock lock(m_mutex);
	bool bRet = m_conn.Connect(sdbus::string(m_sBrokerUrl), sdbus::string(ZMQ_ENGINE));
	this->Init(ZMQ_TOPIC_MNG_CLIENT, sdbus::DeliverDirect);
	m_conn.AddHandler(this);
}

void CZmqTestClientFrame::OnMsgs(const sdbus::Message ** msgs, int count)
{
	const sdbus::Message::Map& value = (*msgs)->MapValue();
	//ͨ����ȡmap���ݾ����ַ���ʽ
	CMsgQueueStruct unit;
	sdbus::string strId;
	sdbus::string strClientSendTime;
	sdbus::string strServerSendTime;
	sdbus::string strServerRecvTime;
	value.GetString(11, strId);
	unit.m_nId = atoi(strId.c_str());
	value.GetString(12, strClientSendTime);
	unit.m_nClientSendTime = atoll(strClientSendTime.c_str());
	unit.m_nClientRecvTime = GetHighTimeRes().GetPerformanceCounter();
	value.GetString(14, strServerSendTime);
	unit.m_nServerSendTime = atoll(strServerSendTime.c_str());
	value.GetString(15, strServerRecvTime);
	unit.m_nServerRecvTime = atoll(strServerRecvTime.c_str());
	GetClientMessageQueue().PushBackAck(unit); //Ӧ��������
	//���·�����Ϣ
	PickMsgFromQueueAndSend();
}

bool CZmqTestClientFrame::ProcessSingleton()
{
	m_hProcMutex = CreateMutexA(NULL, TRUE, GetProcessMutexName().c_str());
	if (!m_hProcMutex) return false;
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return false;
	}
	m_bNeedReleaseMutex = true;
	return true;
}
unsigned int __stdcall GuardThread(PVOID pM)
{
	unsigned int nRet = 0;
	sdbus_thread_name("Guard_Client");
	CZmqTestClientFrame* pThis = (CZmqTestClientFrame*)pM;

	while (!pThis->m_bStopGuard)
	{
		int nTimeCnt = 10;
		for (int i = 0; i < nTimeCnt; ++i)
		{
			Sleep(20000);
			std::cout << GetClientMessageQueue().GetRecvQueueCount() << std::endl;
			if (pThis->ReceiveAll())
			{
				//��ȡ������Ӧ��֮��ֱ���˳�
				SetEvent(pThis->m_hExitEvent);
				break;
			}
			if (pThis->m_bStopGuard)
			{
				break;
			}
		}
	}
	return nRet;
}
void CZmqTestClientFrame::StartGuardThread()
{
	m_bStopGuard = false;
	m_hGuardThread = (HANDLE)_beginthreadex(NULL, 0, GuardThread, this, 0, NULL);
}

void CZmqTestClientFrame::StopGuardThread()
{
	m_bStopGuard = true;
	WaitForSingleObject(m_hGuardThread, 1000);
}

