/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ZmqTestServerFrame.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/27
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#ifndef ZMQTESTSERVERFRAME_H_40564B7A_6BAF_4B45_8D96_54923696E46F
#define ZMQTESTSERVERFRAME_H_40564B7A_6BAF_4B45_8D96_54923696E46F

#include "sdbus\callback.h"
#include "sdbus\connection.h"
#include "engine\zmq_engine.h"
#include "BaseStringType.h"
#include <boost\thread.hpp>


class CZmqTestServerFrame :public sdbus::Callback
{
public:
	CZmqTestServerFrame();
	virtual ~CZmqTestServerFrame();

public:
	void Run();
	bool ReceiveAll();
	void PickMsgAndSend();
	void SendCedaAndPick();

protected:
	t_string GetExitEventName();

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