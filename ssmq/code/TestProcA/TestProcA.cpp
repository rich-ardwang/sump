// TestProcA.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "SSMQ\ISSMQMng.h"
#include "SSMQ\ICedaATSMQMng.h"
#include "SSMQ\ISSMQDefine.h"
#include "ProcAObs.h"
#include "ProcAObsforPost.h"
#include "ProcAFrame.h"

using namespace std;

void publishMsg() {
    sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(1, "heartbeat");
	//values.SetInt32(2, 888999);
	values.SetString(3, "Hello, I'm QB.");
	GetSSMQMng()->Publish("qm.status", msg);
}

void postMsg() {
    sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(1, "heartbeat");
	//values.SetInt32(2, 888999);
	values.SetString(3, "Hi, I'm QB.");
    GetSSMQMng()->Post("qm.status2", msg);
}

/*
void printConnStatus() {
    if (GetSSMQMng()->isConnectBroker()) {
        cout << "is connect" << endl;
    }
    else {
        cout << "is not connect" << endl;
    }
}
*/
class MyTopicObs : public CedaATSMQ::CedaTopicObs {
public:
    MyTopicObs() {}
    ~MyTopicObs() {}

    void onMessage(const sdbus::string topic, const sdbus::string content, int len);
};

void MyTopicObs::onMessage(const sdbus::string topic, const sdbus::string content, int len)
{
    cout << "topic:[" << topic << "]." << endl;
    cout << "content:[" << content << "]." << endl;
    cout << "len:[" << len << "]." << endl;
}

MyTopicObs g_TopicObs;

class MyEventObs : public CedaATSMQ::CedaEventObs {
public:
    MyEventObs() {}
    ~MyEventObs() {}

    void OnEvernt(const int nEventID, const std::string sMsg);
};

void MyEventObs::OnEvernt(const int nEventID, const std::string sMsg)
{
    cout << "nEventID:[" << nEventID << "], sMsg:[" << sMsg << "]."<< endl;
    if (CEDA_LOGIN_OK == nEventID) {
        cout << "ceda login ok." << endl;
        GetCedaATSMQMng()->Subscribe(&g_TopicObs, "idc.cdh.q.5.180205.IB", "CDHSvr");
    }
}



int _tmain(int argc, _TCHAR* argv[])
{
    cout << "QB start..." << endl;

    bool ret = false;
    //ret = GetSSMQMng()->InitServer();
    MyEventObs myObs;
    ret = GetCedaATSMQMng()->InitCeda(&myObs);
    if (ret) {
        cout << "init ceda success." << endl;
    }
    else {
        cout << "init ceda failed!" << endl;
    }
    string sLogin = "{\"method\":\"login\",\"parameter\":{\"password\":\"123456\",\"userName\":\"lance.zhao\"}}";
    //string sLogin = "";
    GetCedaATSMQMng()->LoginCeda(sLogin);

    Sleep(5000);
    cout << "[executionReportQuery] send message." << endl;
    sdbus::string topic = "executionReportQuery";
    sdbus::string reqContent = "{\"deskNames\" : \"BOND\",\"pageNum\":0,\"pageSize\":100}";
    sdbus::string srvID = "STNSSvr";
    sdbus::string response;
    ret = GetCedaATSMQMng()->Send(topic, reqContent, response, srvID, 500);
    if (ret) {
        cout << "async send success." << endl;
        cout << "response:" << response << endl;
    }
    else {
        cout << "async send failed!" << endl;
    }

    CProcAStatus *pObs = new CProcAStatus();
    GetSSMQMng()->RegObs(pObs);
    CProcAFrame *pFram = new CProcAFrame();
    GetSSMQMng()->SetMngCallback(pFram);

    while(true) {
        postMsg();
        publishMsg();
        Sleep(5000);
    }

    /*
    CProcAStatus *pObs = new CProcAStatus();
    GetSSMQMng()->RegObs(pObs);
    CProcAFrame *pFram = new CProcAFrame();
    GetSSMQMng()->SetMngCallback(pFram);

    while (true) {
        postMsg();
        publishMsg();
        Sleep(30000);
        cout << "i am proc A" << endl;
    }
    */

    system("pause");
	return 0;
}

