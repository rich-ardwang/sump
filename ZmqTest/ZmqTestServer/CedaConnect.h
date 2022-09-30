/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  CedaConnect.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/15
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
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

	void			RequestBondList(LONGLONG & reqTime, LONGLONG & ackTime); //仅测试请求应答模式
	void			RequestBondPrimary(LONGLONG& reqTime, LONGLONG& ackTime);

private:
	CedaProxy		m_cedaProxy;	//ceda管理端
	CCedaObserve	m_cedaObserver; //监听器
};

CCedaConnect& GetCedaConnet();



#endif