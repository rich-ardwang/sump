// ZmqTestClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClientMsgQueue.h"
#include "ZmqTestClientFrame.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

int _tmain(int argc, _TCHAR* argv[])
{
	static int m_globalQueueCount = 1000;
	for (int i = 0; i < m_globalQueueCount; ++i)
	{
		CMsgQueueStruct unit;
		unit.m_nId = i;
		GetClientMessageQueue().PushBackReq(unit);
	}
	GetClientMessageQueue().SetReqSize(m_globalQueueCount);

	CZmqTestClientFrame frame;
	frame.Run();

	std::ifstream fRead("..\\statistic\\LocalDataBusAnalysis.csv", std::ios::in);
	if (fRead && fRead.is_open()) {
		fRead.close();
		std::cout << "Open File！" << std::endl;
	}
	//输出接收队列相关消息到csv文件，并在代码中计算时间占比，方便进入excel之后作图分析
	LONGLONG freq = GetHighTimeRes().GetPerformanceFrequency();
	std::cout << "End Programme！" << std::endl;
	std::ofstream fout("..\\statistic\\LocalDataBusAnalysis.csv", std::ios::out | std::ios::trunc);
	if (fout && fout.is_open()){
		std::cout << "Begin Write File！" << std::endl;
		char buf[1000];
		memset(buf, 0, sizeof(buf));
		fout << "统计序号,client发送时间(count),client接收时间(count),server发送时间(count),server接收时间(count),本地总线间隔时间(us),占比百分比(%),服务端间隔时间(us),占比百分比(%),对比百分比1 L/S(%),对比百分比2 S/L(%)" << std::endl;
		for (int i = 0; i < m_globalQueueCount; ++i)
		{
			memset(buf, 0, sizeof(buf));
			CMsgQueueStruct unit;
			GetClientMessageQueue().PopFrontAck(unit);
			double nClientTimeGap = (double)(unit.m_nClientRecvTime - unit.m_nClientSendTime) / (double)freq * 1000000.0;
			double nServerTimeGap = (double)(unit.m_nServerRecvTime - unit.m_nServerSendTime) / (double)freq * 1000000.0;
			double nLocalDataBusTimeGap = (double)(nClientTimeGap - nServerTimeGap);
			double dPercentLocal = 1.0;
			double dPercentServer = 1.0;
			double dPercent1 = 1.0;
			double dPercent2 = 1.0;
			if (nServerTimeGap != 0){
				dPercent1 = (double)nLocalDataBusTimeGap / nServerTimeGap * 100;
			}
			if (nClientTimeGap != 0){
				dPercentLocal = (double)nLocalDataBusTimeGap / nClientTimeGap * 100;
				dPercentServer = (double)nServerTimeGap / nClientTimeGap * 100;
			}
			if (nLocalDataBusTimeGap != 0) {
				dPercent2 = (double)nServerTimeGap / nLocalDataBusTimeGap * 100;
			}
			sprintf_s(buf, "%I64d,%I64d,%I64d,%I64d,%I64d,%f,%f,%f,%f,%f,%f", unit.m_nId, unit.m_nClientSendTime, unit.m_nClientRecvTime, unit.m_nServerSendTime, unit.m_nServerRecvTime,
				nLocalDataBusTimeGap, dPercentLocal, nServerTimeGap, dPercentServer, dPercent1, dPercent2);
			fout << buf << std::endl;
		}
		fout.close();
	}

	return 0;
}

