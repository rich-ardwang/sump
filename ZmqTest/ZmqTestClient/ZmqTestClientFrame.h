/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ZmqTestClientFrame.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/27
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#ifndef ZMQTESTCLIENTFRAME_H_810941B6_416D_4C93_8F39_9C7D2E3F4B1C
#define ZMQTESTCLIENTFRAME_H_810941B6_416D_4C93_8F39_9C7D2E3F4B1C

#include "sdbus\callback.h"
#include "sdbus\connection.h"
#include "engine\zmq_engine.h"
#include "BaseStringType.h"
#include <boost\thread.hpp>

class CZmqTestClientFrame : public sdbus::Callback
{
public:
	CZmqTestClientFrame();
	virtual ~CZmqTestClientFrame();

public:
	void Run();
	void PickMsgFromQueueAndSend();
	bool ReceiveAll();
protected:
protected:
	t_string GetExitEventName();
	t_string GetProcessMutexName();

	//订阅管理消息
	void BeginListenMngMsg();
	void OnMsgs(const sdbus::Message ** msgs, int count);
	bool ProcessSingleton();
	void StartGuardThread();
	void StopGuardThread();

public:
	volatile bool m_bStopGuard;
	HANDLE		m_hExitEvent;
private:
	t_string	m_sBrokerUrl;
	int			m_nMsgQueueSize;

	engine::zmq::EngineInitializer	m_engineInit;
	sdbus::Connection	m_conn;
	HANDLE				m_hProcMutex;
	HANDLE				m_hGuardThread;
	bool				m_bNeedReleaseMutex;
	boost::mutex		m_mutex;
};



#endif