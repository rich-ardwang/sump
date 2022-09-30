/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ClientMsgQueue.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#ifndef CLIENTMSGQUEUE_H_2E501C29_BC7A_43CE_99F2_C2220EEE457E
#define CLIENTMSGQUEUE_H_2E501C29_BC7A_43CE_99F2_C2220EEE457E
#include "MsgQueueStruct.h"
#include <queue>

// ��ʱ��������û���߳���ش���
class CClientMsgQueue
{
public:
	CClientMsgQueue();
	~CClientMsgQueue();
	void SetReqSize(int nCount);

	void PushBackReq(CMsgQueueStruct& unit);
	bool PopFrontReq(CMsgQueueStruct& unit);

	void PushBackAck(CMsgQueueStruct& unit);
	void PopFrontAck(CMsgQueueStruct& unit);

	bool ReceiveAll();
	int  GetRecvQueueCount();

protected:
	std::queue<CMsgQueueStruct> m_msgQueueReq;
	std::queue<CMsgQueueStruct> m_msgQueueAck;
	volatile int				m_reqSize;
	CRITICAL_SECTION		m_cs;
};

CClientMsgQueue& GetClientMessageQueue();


#endif