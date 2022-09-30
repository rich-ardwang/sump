/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  ServerMsgQueue.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#include "stdafx.h"
#include "ServerMsgQueue.h"
#include "..\ZmqTestClient\TestCSLock.h"

CServerMsgQueue::CServerMsgQueue()
{
	InitializeCriticalSection(&m_cs);
}

CServerMsgQueue::~CServerMsgQueue()
{
	DeleteCriticalSection(&m_cs);
}

void CServerMsgQueue::SetMsgQueueCount(int nQueueCount)
{
	m_nQueueCout = nQueueCount;
}

void CServerMsgQueue::PushBack(CMsgQueueStruct& unit)
{
	CTestCSLock lock(&m_cs);
	m_msgQueue.push(unit);
}

void CServerMsgQueue::PopFront(CMsgQueueStruct& unit)
{
	CTestCSLock lock(&m_cs);
	if (m_msgQueue.empty()) return;
	unit = m_msgQueue.front();
	m_msgQueue.pop();
}

void CServerMsgQueue::PushBackSend(CMsgQueueStruct& unit)
{
	CTestCSLock lock(&m_cs);
	m_msgQueueSend.push(unit);
}

bool CServerMsgQueue::ReceiveAll()
{
	CTestCSLock lock(&m_cs);
	return (m_msgQueueSend.size() == m_nQueueCout);
}

int CServerMsgQueue::QueueCout()
{
	CTestCSLock lock(&m_cs);
	return m_msgQueue.size();
}

CServerMsgQueue& GetServerMsgQueue()
{
	static CServerMsgQueue globalMsgQueue;
	return globalMsgQueue;
}
