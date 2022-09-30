/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ServerMsgQueue.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#ifndef SERVERMSGQUEUE_H_80B990DA_73FA_491A_8369_6F728986AF86
#define SERVERMSGQUEUE_H_80B990DA_73FA_491A_8369_6F728986AF86
#include "..\ZmqTestClient\MsgQueueStruct.h"
#include <queue>

class CServerMsgQueue
{
public:
	CServerMsgQueue();
	~CServerMsgQueue();

	void SetMsgQueueCount(int nQueueCount);
	void PushBack(CMsgQueueStruct& unit);
	void PopFront(CMsgQueueStruct& unit);
	void PushBackSend(CMsgQueueStruct& unit);

	bool ReceiveAll();
	int  QueueCout();
private:
	std::queue<CMsgQueueStruct>	m_msgQueue;
	std::queue<CMsgQueueStruct>	m_msgQueueSend;
	CRITICAL_SECTION		m_cs;
	volatile	int			m_nQueueCout;
};

CServerMsgQueue& GetServerMsgQueue();

#endif