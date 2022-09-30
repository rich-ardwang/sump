/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  TestCSLock.cpp
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
#include "TestCSLock.h"

CTestCSLock::CTestCSLock(CRITICAL_SECTION* pcs)
{
	m_pcs = pcs;
	EnterCriticalSection(m_pcs);
}

CTestCSLock::~CTestCSLock()
{
	LeaveCriticalSection(m_pcs);
}
