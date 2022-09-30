/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  CedaObserve.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/15
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
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