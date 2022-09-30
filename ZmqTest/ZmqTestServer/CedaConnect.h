/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  CedaConnect.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/15
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#ifndef CEDACONNECT_H_385100EF_FF2C_4F2F_87FD_149DBDD0F471
#define CEDACONNECT_H_385100EF_FF2C_4F2F_87FD_149DBDD0F471
#include "CedaApi.h"
#include "CedaObserve.h"
class CCedaConnect
{
public:
	CCedaConnect();
	virtual ~CCedaConnect();

	void			InitialCeda(const char* account, const char* pwd);

	void			RequestBondList(LONGLONG & reqTime, LONGLONG & ackTime); //����������Ӧ��ģʽ
	void			RequestBondPrimary(LONGLONG& reqTime, LONGLONG& ackTime);

private:
	CedaProxy		m_cedaProxy;	//ceda�����
	CCedaObserve	m_cedaObserver; //������
};

CCedaConnect& GetCedaConnet();



#endif