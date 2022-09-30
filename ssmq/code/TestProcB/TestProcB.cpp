// TestProcB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "SSMQ\ISSMQMng.h"
#include "SSMQ\ICedaATSMQMng.h"
#include "SSMQ\ISSMQDefine.h"
#include "ProcBObs.h"
#include "ProcBObsforPost.h"
#include "ProcBFrame.h"

using namespace std;


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
        GetCedaATSMQMng()->Subscribe(&g_TopicObs, "idc.base.allholiday", "BaseSvr");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    cout << "QM start..." << endl;

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

    CProcBStatus *pObs = new CProcBStatus();
    GetSSMQMng()->RegObs(pObs);
    CProcBPost *pObsPost = new CProcBPost();
    GetSSMQMng()->RegObs(pObsPost);
    CProcBFrame *pFram = new CProcBFrame();
    GetSSMQMng()->SetMngCallback(pFram);

    while(true) {
        Sleep(5000);
        sdbus::Message msg;
        sdbus::Message::Map& values = msg.MapValue();
        values.SetString(1, "heartbeat");
        values.SetInt32(2, 888999);
        values.SetString(3, "Hello, I'm QM.");
        GetSSMQMng()->Publish("qm.status.qb", msg);
    }




    /*
    cout << "initServer start..." << endl;
    GetSSMQMng()->InitServer();
    CProcBStatus *pObs = new CProcBStatus();
    GetSSMQMng()->RegObs(pObs);
    CProcBPost *pObsPost = new CProcBPost();
    GetSSMQMng()->RegObs(pObsPost);
    CProcBFrame *pFram = new CProcBFrame();
    GetSSMQMng()->SetMngCallback(pFram);
    sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(1, "heartbeat");
	values.SetInt32(2, 888999);
	values.SetString(3, "hello world");
	GetSSMQMng()->Publish("qm.status", msg);
    while (true) {
        Sleep(5000);
        cout << "i am proc B" << endl;
    }
    */


    system("pause");
	return 0;
}

