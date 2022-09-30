/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ClientMsgQueue.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#ifndef CLIENTMSGQUEUE_H_2E501C29_BC7A_43CE_99F2_C2220EEE457E
#define CLIENTMSGQUEUE_H_2E501C29_BC7A_43CE_99F2_C2220EEE457E
#include "MsgQueueStruct.h"
#include <queue>

// 暂时不加锁，没有线程相关处理
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