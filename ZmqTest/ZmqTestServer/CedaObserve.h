/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  CedaObserve.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/15
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#ifndef CEDAOBSERVE_H_19576C6C_406B_44A0_9ABC_2C2AFA2E6092
#define CEDAOBSERVE_H_19576C6C_406B_44A0_9ABC_2C2AFA2E6092
#include "CedaApi.h"

class CCedaObserve : public CedaClient
{
public:
	void OnMessage(const char* topic, const char* content, int len);
	void onEvent(int nevent);
};


#endif