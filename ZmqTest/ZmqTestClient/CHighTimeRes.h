/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  CHighTimeRes.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/04/01
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#ifndef CHIGHTIMERES_H_581609CB_4EA5_4E40_A0C4_90243B054CA3
#define CHIGHTIMERES_H_581609CB_4EA5_4E40_A0C4_90243B054CA3

class CHighTimeRes
{
public:
	CHighTimeRes();
	LONGLONG GetPerformanceCounter();
	LONGLONG GetPerformanceFrequency();
private:
	LONGLONG longTime;
};

CHighTimeRes& GetHighTimeRes();

#endif