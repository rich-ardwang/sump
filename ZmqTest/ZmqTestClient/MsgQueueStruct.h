/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  MsgQueueStruct.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#ifndef MSGQUEUESTRUCT_H_3D30309E_F635_4B4A_B48F_E968C563730D
#define MSGQUEUESTRUCT_H_3D30309E_F635_4B4A_B48F_E968C563730D
struct CMsgQueueStruct
{
	LONGLONG m_nId;
	LONGLONG m_nClientSendTime;
	LONGLONG m_nClientRecvTime;
	LONGLONG m_nServerSendTime;
	LONGLONG m_nServerRecvTime;
	CMsgQueueStruct()
	{
		m_nClientSendTime = 0;
		m_nClientRecvTime = 0;
		m_nServerSendTime = 0;
		m_nServerRecvTime = 0;
		m_nId = 0;
	}
	CMsgQueueStruct& operator=(const CMsgQueueStruct& outer)
	{
		if (this == &outer) return *this;
		m_nClientSendTime = outer.m_nClientSendTime;
		m_nClientRecvTime = outer.m_nClientRecvTime;
		m_nServerSendTime = outer.m_nServerSendTime;
		m_nServerRecvTime = outer.m_nServerRecvTime;
		m_nId = outer.m_nId;
		return *this;
	}
};



#endif