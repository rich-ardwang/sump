/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  TestCSLock.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#ifndef TESTCSLOCK_H_CD7FC39C_8EC3_4590_9652_8E2592431AC5
#define TESTCSLOCK_H_CD7FC39C_8EC3_4590_9652_8E2592431AC5

class CTestCSLock
{
public:
	CTestCSLock(CRITICAL_SECTION* pcs);
	~CTestCSLock();
private:
	CRITICAL_SECTION*	m_pcs;
};

#endif