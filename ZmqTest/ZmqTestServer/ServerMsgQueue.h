/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ServerMsgQueue.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
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