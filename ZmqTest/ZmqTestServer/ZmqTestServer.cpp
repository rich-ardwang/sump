// ZmqTestServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CedaConnect.h"
#include "ZmqTestServerFrame.h"
#include "ServerMsgQueue.h"
#include <iostream>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
	std::string strUserName = "lance.zhao";
	std::string strPwd = "123456";

	static int m_globalQueueCount = 1000;
	GetServerMsgQueue().SetMsgQueueCount(m_globalQueueCount);

	GetCedaConnet().InitialCeda(strUserName.c_str(), strPwd.c_str());

	CZmqTestServerFrame frame;
	frame.Run();

	return 0;
}

