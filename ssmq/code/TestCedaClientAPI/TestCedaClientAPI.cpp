// TestCedaClientAPI.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cJSON.h"


using namespace std;

/*
测试我对CedaApi封装好的CedaBusManager与CCedaAgent
如果是单Ceda连接的话，CedaBusManager只创建一个CedaAgent，它还可以扩展管理其他数据通道(考虑到未来可能的需求)，
CedaBusManager是线程安全的singleton，相当于一个数据通道的工厂
*/
#include "CedaBusManager.h"
#include <boost/thread.hpp>


/*
void t1() {
    std::string sLogin = "{\"method\":\"login\",\"parameter\":{\"password\":\"123456\",\"userName\":\"lance.zhao\"}}";
    CCedaAgent *pA = GET_CEDA_BUS_MANAGER.CreateCedaClient(sLogin);
    GET_CEDA_BUS_MANAGER.removeCedaClient(pA);
    cout << "t1" << endl;
}

void t2() {
    std::string sLogin = "{\"method\":\"login\",\"parameter\":{\"password\":\"123456\",\"userName\":\"lance.zhao\"}}";
    CCedaAgent *pA = GET_CEDA_BUS_MANAGER.CreateCedaClient(sLogin);
    GET_CEDA_BUS_MANAGER.removeCedaClient(pA);
    cout << "t2" << endl;
}

void t3() {
    string str1 = "31";
    string str2 = "32";
    CCedaAgent *pA = GET_CEDA_BUS_MANAGER.CreateCedaClient(str1, str2);
    GET_CEDA_BUS_MANAGER.removeCedaClient(pA);
    cout << "t3" << endl;
}

void t4() {
    string str1 = "41";
    string str2 = "42";
    CCedaAgent *pA = GET_CEDA_BUS_MANAGER.CreateCedaClient(str1, str2);
    GET_CEDA_BUS_MANAGER.removeCedaClient(pA);
    cout << "t4" << endl;
}


void t5() {
    PxyInfo p51;
    CCedaAgent *pA = GET_CEDA_BUS_MANAGER.CreateCedaClient(p51);
    GET_CEDA_BUS_MANAGER.removeCedaClient(pA);
    cout << "t5" << endl;
}

void t6() {
    PxyInfo p61;
    CCedaAgent *pA = GET_CEDA_BUS_MANAGER.CreateCedaClient(p61);
    GET_CEDA_BUS_MANAGER.removeCedaClient(pA);
    cout << "t6" << endl;
}
*/

/*
string paseJson(const char* content, const char *sFiled) {
    string sRet;
    cJSON* pRoot = cJSON_Parse(content);
    if (pRoot) {
        cJSON* objJsonItem = NULL;
        objJsonItem = cJSON_GetObjectItem(pRoot, sFiled);
        if (objJsonItem) {
            sRet = objJsonItem->valuestring;
        }
        cJSON_Delete(pRoot);
    }
    else {
        cout << "can't parse to json" << endl;
    }
    return sRet;
}

void CCedaObserve::onMessage(const char* topic, const char* content, int len) {
    //Msg obs
    string sRecvTopic(topic);
    string sContent(content);
    cout << "recvTopic=[" << sRecvTopic << "]" << endl;
    cout << "content=[" << sContent << "]" << endl;
    cout << "len=[" << len << "]" << endl;

    string sSid = paseJson(content, "sid");
    if (!sSid.empty()) {
        string subTopic = "idc.connect.zf3sMw21+I0+OmrlpIbAKA==";
        subTopic += sSid;
        cout << "subTopic=[" << subTopic << "]" << endl;
        subscribe(const_cast<char *>(subTopic.c_str()), "ConnectSvr");
    }
}

void CCedaObserve::onEvent(int eventID) {
    cout << "eventID=" << eventID << endl;
    char *ret = NULL;
    string s;
    switch (eventID) {
    case CONNECTION_CONNECTED:
        break;
    case CONNECTION_CLOSED:
        break;
    case CONNECTION_LOGIN_SUCCESS:
        //send接口没有回来的Msg和Event，所以不清楚是否post成功
        //send("executionReportQuery", "{\"deskNames\" : \"BOND\",\"pageNum\":0,\"pageSize\":100}", 1);
        //request完全阻塞，不能使用

        //综上，cedaClient基本上仅能使用subscribe与asyncRequest接口
        subscribe("idc.cdh.q.5.180205.IB", "CDHSvr");
        subscribe("idc.executionReport.658f222a5d954f90a6547b00d068758e.940efae4194b49e7825e1a9a84528aef", "STNSSvr");
        
        //unSubscribe("idc.cdh.q.5.180205.IB");
        asyncRequest("executionReportQuery", "{\"deskNames\" : \"BOND\",\"pageNum\":0,\"pageSize\":100}", "STNSSvr", 1);


        break;
    default:
        break;
    }
}
*/


int _tmain(int argc, _TCHAR* argv[])
{
    /*
    boost::thread thrd1(&t1);
    boost::thread thrd2(&t2);
    boost::thread thrd3(&t3);
    boost::thread thrd4(&t4);
    boost::thread thrd5(&t5);
    boost::thread thrd6(&t6);
    thrd1.join();
    thrd2.join();
    thrd3.join();
    thrd4.join();
    thrd5.join();
    thrd6.join();
    */

    /* 下面是对CedaApi.h中dll导出类的测试，connect时发生崩溃 */
    /*
    CedaProxy cedaProxy;
    cedaProxy.setLogLevel(3);
    cedaProxy.setLogFile(".\\testCedaApi");
    CCedaObserve cedaObs;
    cedaProxy.addClient(&cedaObs);
    string sLogin = "{\"method\":\"login\",\"parameter\":{\"password\":\"123456\",\"userName\":\"lance.zhao\"}}";
    cedaObs.setLoginMessage(const_cast<char *>(sLogin.c_str()));
    cedaObs.connect("172.16.97.106", 9060, 1);

    while (true) {
        cedaObs.asyncRequest("executionReportQuery", "{\"deskNames\" : \"BOND\",\"pageNum\":0,\"pageSize\":100}", "STNSSvr", 1);
        Sleep(5000*4);
    }
    */

    /*
    CedaProxy cedaProxy;
    cedaProxy.setLogLevel(3);
    cedaProxy.setLogFile(".\\testCedaApi");
    CCedaAgent *pCedaAgt = new CCedaAgent();
    cedaProxy.addClient(pCedaAgt);
    std::string sLogin = "{\"method\":\"login\",\"parameter\":{\"password\":\"123456\",\"userName\":\"lance.zhao\"}}";
    if (pCedaAgt->InitCeda(sLogin)) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientB success.\r\n");
        cout << "init ceda okok." << endl;
        
        pCedaAgt->subscribe("idc.base.allholiday", "BaseSvr");
        pCedaAgt->subscribe("idc.cdh.q.5.180205.IB", "CDHSvr");
        //return pCedaAgt;
    }
    if (pCedaAgt->StopCeda()) {
        cout << "stop ceda okok." << endl;
    }
    */

    std::string sLogin = "{\"method\":\"login\",\"parameter\":{\"password\":\"123456\",\"userName\":\"lance.zhao\"}}";
    CCedaAgent *pA = GET_CEDA_BUS_MANAGER.CreateCedaClient(sLogin);
    pA->InitCeda(sLogin);
    //pA->subscribe("idc.base.allholiday", "BaseSvr");
    pA->subscribe("idc.cdh.q.5.180205.IB", "CDHSvr");
    //GET_CEDA_BUS_MANAGER.removeCedaClient(pA);

    while (true) {
        Sleep(1000*60);
    }
    system("pause");
    return 0;
}

