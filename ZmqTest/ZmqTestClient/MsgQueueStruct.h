/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  MsgQueueStruct.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
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