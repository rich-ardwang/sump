/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  CedaConnect.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/15
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
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
	//#host，port，protocol，1：tcp，2：tcps
	m_cedaObserver.connect("180.169.219.234", 9060, 1); //默认填写,之后配置到文件 //180.169.219.234:9060--172.16.97.106:9060
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
