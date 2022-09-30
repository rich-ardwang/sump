/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  ZmqTestServerFrame.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/27
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#include "stdafx.h"
#include "ZmqTestServerFrame.h"
#include "ZmqTestDefine.h"
#include <Psapi.h>
#include <process.h>
#include "sdbus/helper.h"
#include "ServerMsgQueue.h"
#include "CedaConnect.h"
#include <iostream>

CZmqTestServerFrame::CZmqTestServerFrame()
	: m_sBrokerUrl("tcp://127.0.0.1:5554") //暂定
	, m_nMsgQueueSize(128)
	, m_hProcMutex(NULL)
	, m_bNeedReleaseMutex(false)
	, m_bStopGuard(false)
{
	m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, GetExitEventName().c_str());
}

CZmqTestServerFrame::~CZmqTestServerFrame()
{
	CloseHandle(m_hExitEvent);
	if (m_bNeedReleaseMutex)
	{
		ReleaseMutex(m_hProcMutex);
	}
	CloseHandle(m_hProcMutex);
}

void CZmqTestServerFrame::Run()
{
	if (!ProcessSingleton())
	{
		return;
	}
	size_t len = m_sBrokerUrl.length();
	len += 1;
	char* pszUrl = new char[len];
	strcpy_s(pszUrl, len, m_sBrokerUrl.c_str());
	int nRet = engine::zbroker::create_broker(pszUrl, m_sBrokerUrl.length(), m_nMsgQueueSize);
	BeginListenMngMsg();
	StartGuardThread();
	WaitForSingleObject(m_hExitEvent, INFINITE);
	StopGuardThread();
	m_conn.DelHandler(this);
	delete[] pszUrl;
}


bool CZmqTestServerFrame::ReceiveAll()
{
	return GetServerMsgQueue().ReceiveAll();
}


t_string CZmqTestServerFrame::GetExitEventName()
{
	return "Sumscope_ZmqTestServer_Exit";  //暂时写成固定
}

void CZmqTestServerFrame::BeginListenMngMsg()
{
	boost::mutex::scoped_lock lock(m_mutex);
	bool bRet = m_conn.Connect(sdbus::string(m_sBrokerUrl), sdbus::string(ZMQ_ENGINE));
	this->Init(ZMQ_TOPIC_MNG, sdbus::DeliverDirect);
	m_conn.AddHandler(this);
}

static int nGap = 1;
void CZmqTestServerFrame::OnMsgs(const sdbus::Message ** msgs, int count)
{
	const sdbus::Message::Map& values = (*msgs)->MapValue();
	CMsgQueueStruct unit;
	unit.m_nServerSendTime = GetHighTimeRes().GetPerformanceCounter();
	sdbus::string strId;
	sdbus::string strClientSendTime;
	values.GetString(11, strId);
	unit.m_nId = atoll(strId.c_str());
	values.GetString(12, strClientSendTime);
	unit.m_nClientSendTime = atoll(strClientSendTime.c_str());
	GetServerMsgQueue().PushBack(unit);
	SendCedaAndPick();
	//boost::thread thrd(boost::bind(&CZmqTestServerFrame::SendCedaAndPick, this));
	//thrd.join();
	//unit.m_nServerRecvTime = GetTickCount();
	//直接发送消息
	nGap++;
}

void CZmqTestServerFrame::PickMsgAndSend()
{
	CMsgQueueStruct unit;
	sdbus::Message msg;
	GetServerMsgQueue().PopFront(unit);

	unit.m_nServerRecvTime = GetHighTimeRes().GetPerformanceCounter();
	char buf[100];
	_i64toa_s(unit.m_nId, buf,_countof(buf), 10);
	msg.SetString(11, buf);
	_i64toa_s(unit.m_nClientSendTime, buf,_countof(buf), 10);
	msg.SetString(12, buf);
	_i64toa_s(unit.m_nServerSendTime, buf, _countof(buf),10);
	msg.SetString(14, buf);
	_i64toa_s(unit.m_nServerRecvTime, buf,_countof(buf), 10);
	msg.SetString(15, buf);
	m_conn.Send(ZMQ_TOPIC_MNG_CLIENT, msg);
	GetServerMsgQueue().PushBackSend(unit);
}

void CZmqTestServerFrame::SendCedaAndPick()
{
	CMsgQueueStruct unit;
	sdbus::Message msg;
	GetServerMsgQueue().PopFront(unit);

	if (true){
		GetCedaConnet().RequestBondList(unit.m_nServerSendTime, unit.m_nServerRecvTime);
	}
	else{
		GetCedaConnet().RequestBondPrimary(unit.m_nServerSendTime, unit.m_nServerRecvTime);
	}
	unit.m_nServerRecvTime = GetHighTimeRes().GetPerformanceCounter();
	char buf[100];
	_i64toa_s(unit.m_nId, buf,_countof(buf), 10);
	msg.SetString(11, buf);
	_i64toa_s(unit.m_nClientSendTime, buf,_countof(buf), 10);
	msg.SetString(12, buf);
	_i64toa_s(unit.m_nServerSendTime, buf,_countof(buf), 10);
	msg.SetString(14, buf);
	_i64toa_s(unit.m_nServerRecvTime, buf,_countof(buf), 10);
	msg.SetString(15, buf);
	m_conn.Send(ZMQ_TOPIC_MNG_CLIENT, msg);
	GetServerMsgQueue().PushBackSend(unit);
}

bool CZmqTestServerFrame::ProcessSingleton()
{
	m_hProcMutex = CreateMutexA(NULL, TRUE, m_sBrokerUrl.c_str());
	if (!m_hProcMutex){
		return false;
	}
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
	sdbus_thread_name("Guard");
	CZmqTestServerFrame *pThis = (CZmqTestServerFrame*)pM;

	//检测是否收完所有消息
	while (!pThis->m_bStopGuard)
	{
		int nTimeCnt = 10;
		for (int i = 0; i < nTimeCnt; ++i)
		{
			Sleep(20000);
			std::cout << GetServerMsgQueue().QueueCout() << std::endl;
			if (pThis->ReceiveAll())
			{
				Sleep(10000);
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
void CZmqTestServerFrame::StartGuardThread()
{
	m_bStopGuard = false;
	m_hGuardThread = (HANDLE)_beginthreadex(NULL, 0, GuardThread, this, 0, NULL);
}

void CZmqTestServerFrame::StopGuardThread()
{
	m_bStopGuard = true;
	WaitForSingleObject(m_hGuardThread, 1000);
}
