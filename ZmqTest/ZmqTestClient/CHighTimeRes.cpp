/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  CHighTimeRes.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/04/01
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#include "stdafx.h"
#include "CHighTimeRes.h"

CHighTimeRes::CHighTimeRes()
{
	longTime = GetPerformanceCounter();
}

LONGLONG CHighTimeRes::GetPerformanceCounter()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

LONGLONG CHighTimeRes::GetPerformanceFrequency()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	return freq.QuadPart;
}

CHighTimeRes& GetHighTimeRes()
{
	static CHighTimeRes res;
	return res;
}
