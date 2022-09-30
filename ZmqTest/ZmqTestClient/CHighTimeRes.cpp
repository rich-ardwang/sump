/***************************************************************************************************
  *Copyright(C),2010-2016,Sumscope
  *FileName    :  CHighTimeRes.cpp
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/04/01
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
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
