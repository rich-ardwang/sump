#pragma once
#include <set>
#include "sdbus/callback.h"
#include "sdbus/connection.h"
#include "engine/zmq_engine.h"
#include <boost/thread.hpp>
#include <string>
#include "Config.h"
#include "MQServerDef.h"
#include "CedaBusManager.h"
#include "Utils.h"



typedef std::set<std::string> StringSet;


class CCedaAgent;
class CSSMQServerFrame : public sdbus::Callback
{
public:
	CSSMQServerFrame(void);
	~CSSMQServerFrame(void);

public:
	void Run();
	bool NoClientRunning();

protected:
	void InitConfig();
	void AddClient(const std::string& sClientName);
	void RemoveClient(const std::string& sClientName);
	void AckProcID(const sdbus::string& sClientPipe);
	//关闭总线以及当前连接的所有client
	void CloseAll(const std::string& sCaller);
    //关闭指定client并释放其余client以确保总线关闭
    void CloseOneAndReleaseOther(const std::string& sCaller);
	void WaitClientAndClose(const std::string& sCaller);
	bool NoClientRunningButCaller(const std::string& sCaller);
	t_string GetExitEventName();
	//订阅管理消息
	void BeginListenMngMsg();
	void OnMsgs(const sdbus::Message ** msgs , int count);
	bool ProcessSingleton();
	void StartGuardThread();
	void StopGuardThread();
	BOOL FindProcessByProcID(DWORD dwProcID);

    //ceda business worker
    boost::thread *CreateThreadWorker(void (CSSMQServerFrame::*fn)());
    void MngReqRun();
    void MngReqStop();
    void MngReqJoin();
    void SubReqRun();
    void SubReqStop();
    void SubReqJoin();
    void DispachMsgRun();
    void DispachMsgStop();
    void DispachMsgJoin();

    //ceda business handle
    void HandleMngReqTask();
    void HandleSubReqTask();
    void HandleDispachMsgTask();

    //ceda internal functions
    sdbus::string getCedaBindKey(const sdbus::string &cedaHeader, const sdbus::string &bindKey);
    void cedaInitAck(const string &mngBindKey, const bool ret);
    void cedaLoginAck(const string &eventBindKey, const int ret, const std::string sid);
    void getTopicSet(const std::string &topics, std::set<string> &topicSet);
    void cedaSubAck(const string &eventBindKey, const int ret, const std::string &retMsg);
    void cedaDispatchMsg(const string &topicBindKey, const CedaDisMsg &disMsg);

public:
    void putCedaDispatch(CedaDisMsg *pDis);

public:
	volatile bool m_bStopGuard;
	HANDLE m_hExitEvent;

private:
	//本身也算一个client,监听管理消息
	engine::zmq::EngineInitializer m_engineInit;
	sdbus::Connection m_conn;
	HANDLE m_hProcMutex;
	HANDLE m_hGuardThread;
	bool m_bNeedReleaseMutex;
	StringSet m_setClient;
	DWORD m_dwProcID;
	boost::mutex m_mutex;

    //ceda message channel
    CCedaAgent                  *m_pCedaAgent;
    CedaCliObsMap               m_mpCedaMngObs;
    TopicObsMap                 m_mpCedaTopicObs;
    TopicsDestinationMap        m_mpTopicsDest;
    CedaManageReq               m_mngReq;
    CedaSubReq                  m_subReq;
    CedaDispatchMsg             m_disMsgQue;
    CedaSendMsg                 m_sendReq;

    //ceda worker ptr
    boost::thread               *m_pCedaMngReqThread;
    boost::thread               *m_pCedaSubReqThread;
    ThreadVector                m_plstCedaDispachThreads;

    volatile bool m_bEnableAddClient;
};

