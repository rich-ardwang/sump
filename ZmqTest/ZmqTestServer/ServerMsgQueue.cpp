/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  ServerMsgQueue.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
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
