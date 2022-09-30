/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  ClientMsgQueue.cpp
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
#include "ClientMsgQueue.h"
#include "TestCSLock.h"

static CClientMsgQueue globalMsgQueue;

CClientMsgQueue::CClientMsgQueue()
{
	InitializeCriticalSection(&m_cs);
}

CClientMsgQueue::~CClientMsgQueue()
{
	DeleteCriticalSection(&m_cs);
}

void CClientMsgQueue::SetReqSize(int nCount)
{
	m_reqSize = nCount;
}

void CClientMsgQueue::PushBackReq(CMsgQueueStruct& unit)
{
	CTestCSLock lock(&m_cs);
	m_msgQueueReq.push(unit);
}

bool CClientMsgQueue::PopFrontReq(CMsgQueueStruct& unit)
{
	CTestCSLock lock(&m_cs);
	if (m_msgQueueReq.empty()) return false;
	unit = m_msgQueueReq.front();
	m_msgQueueReq.pop();
	return true;
}

void CClientMsgQueue::PushBackAck(CMsgQueueStruct& unit)
{
	CTestCSLock lock(&m_cs);
	m_msgQueueAck.push(unit);
}

void CClientMsgQueue::PopFrontAck(CMsgQueueStruct& unit)
{
	CTestCSLock lock(&m_cs);
	if (m_msgQueueAck.empty()) return;
	unit = m_msgQueueAck.front();
	m_msgQueueAck.pop();
}

bool CClientMsgQueue::ReceiveAll()
{
	CTestCSLock lock(&m_cs);
	return (m_msgQueueAck.size() == m_reqSize);
}

int CClientMsgQueue::GetRecvQueueCount()
{
	CTestCSLock lock(&m_cs);
	return m_msgQueueAck.size();
}

CClientMsgQueue& GetClientMessageQueue()
{
	return globalMsgQueue;
}
