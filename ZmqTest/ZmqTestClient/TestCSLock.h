/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  TestCSLock.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/28
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#ifndef TESTCSLOCK_H_CD7FC39C_8EC3_4590_9652_8E2592431AC5
#define TESTCSLOCK_H_CD7FC39C_8EC3_4590_9652_8E2592431AC5

class CTestCSLock
{
public:
	CTestCSLock(CRITICAL_SECTION* pcs);
	~CTestCSLock();
private:
	CRITICAL_SECTION*	m_pcs;
};

#endif