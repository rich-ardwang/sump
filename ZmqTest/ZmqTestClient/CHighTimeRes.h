/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  CHighTimeRes.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/04/01
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
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