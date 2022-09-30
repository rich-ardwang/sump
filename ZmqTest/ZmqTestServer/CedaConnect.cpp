/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  CedaConnect.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/15
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
***************************************************************************************************/
#include "stdafx.h"
#include "CedaConnect.h"
#include <windows.h>

CCedaConnect::CCedaConnect()
{

}

CCedaConnect::~CCedaConnect()
{

}

void CCedaConnect::InitialCeda(const char* account, const char* pwd)
{
	m_cedaProxy.setLogLevel(0);
	m_cedaProxy.setLogFile("testCedaApi");
	m_cedaProxy.addClient(&m_cedaObserver);
	char strline[256];
	memset(strline, 0, sizeof(strline));
	_snprintf_s(strline, _countof(strline), _TRUNCATE, "{\"method\":\"login\",\"parameter\":{\"password\":\"%s\",\"userName\":\"%s\"}}", pwd, account);
	m_cedaObserver.setLoginMessage(strline);
	//#host��port��protocol��1��tcp��2��tcps
	m_cedaObserver.connect("180.169.219.234", 9060, 1); //Ĭ����д,֮�����õ��ļ� //180.169.219.234:9060--172.16.97.106:9060
}

void CCedaConnect::RequestBondList(LONGLONG & reqTime, LONGLONG & ackTime)
{
	//reqTime = GetTickCount();
	//const char *cout = m_cedaObserver.request("goodsQuery", "{}", "ConnectSvr", 1, 30);
	//m_cedaObserver.send("executionReportQuery", "{\"deskNames\" : \"BOND\",\"pageNum\":0,\"pageSize\":100}", "STNSSvr",1);
	char *cout = m_cedaObserver.request("executionReportQuery", "{\"deskNames\" : \"BOND\",\"pageNum\":0,\"pageSize\":100}", "STNSSvr", 1, 100);
	if (cout != NULL){
		ackTime = GetHighTimeRes().GetPerformanceCounter();
		if (strlen(cout) < 20){
			ackTime = reqTime + 1000;
		}
	}
	else{
		ackTime = reqTime + 1000;
	}
}

void CCedaConnect::RequestBondPrimary(LONGLONG& reqTime, LONGLONG& ackTime)
{
	//reqTime = GetTickCount();
	//m_cedaObserver.send("goodsQuery", "{}", "ConnectSvr", 1);
	const char *cout = m_cedaObserver.request("goodsQuery", "{}", "ConnectSvr", 1, 30);

	//char *cout = m_cedaObserver.request("executionReportQuery", "{\"deskNames\" : \"BOND\",\"pageNum\":0,\"pageSize\":100}", "STNSSvr", 1, 100);
	if (cout != NULL){
		ackTime = GetHighTimeRes().GetPerformanceCounter();
		if (strlen(cout) < 20){
			ackTime = reqTime + 1000;
		}
	}
	else{
		ackTime = reqTime + 1000;
	}
}

CCedaConnect& GetCedaConnet()
{
	static CCedaConnect m_globalCedaConnect;
	return m_globalCedaConnect;
}
