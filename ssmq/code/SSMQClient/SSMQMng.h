#pragma once
#include "ISSMQMng.h"
#include "ISSMQObs.h"
#include "CedaATSMQMng.h"
#include "engine/zmq_engine.h"
#include "sdbus/connection.h"
#include <map>
#include "Config.h"

using namespace SSMQ;

namespace ss
{
    class logger;
}

class CMQCallback;
typedef std::map<sdbus::string, CMQCallback*> mapMQCallback;

class CMQCallback;
typedef std::map<sdbus::string, CMQCallback*> mapCedaMQCallback;

class CSSMQMng : public ISSMQMng, 
				 public ISSMQObs
{
public:
	CSSMQMng(void);
	virtual ~CSSMQMng(void);

public:
	//Interface of ISSMQMng
	bool InitServer();
	bool ReleaseServer();
	void SetMngCallback(ISSMQMngCallback* pCallback);
	void CloseLocalDataBus();
	bool RegObs(ISSMQObs* pObs);
    void ReleaseObs(ISSMQObs* pObs);
    void ReleaseAllObs();
	bool Send(const sdbus::string& bindKey , const sdbus::Message& request , sdbus::Message& response , int timeout = -1);
	bool Post(const sdbus::string& bindKey , const sdbus::Message& msg);
	bool Publish(const sdbus::string& bindKey , const sdbus::Message& msg);
	sdbus::string GetUniqueMsgID();

	//Interface of ISSMQObs
	sdbus::string GetBindKey();
	SSMQDeliverType GetDeliverType();
	void OnMsgs(const sdbus::Message& msgs);

	sdbus::string GetClientName();
	BOOL FindProcessByName(LPCTSTR szFileName);
	bool IsConnectRightSrv();
	void RestartServer();
	void ReConnectServer();

    /* ceda internal function */
    bool isConnectBroker(); //判断是否已经连接了broker，没有必要重复connect
    bool RegCedaObs(ISSMQObs* pObs, const sdbus::string &cedaHeader);
    void UnRegCeda2Server();
    void ReleaseCedaObs(ISSMQObs* pObs, const sdbus::string &cedaHeader);
    void ReleaseAllCedaObs();
    bool ReleaseCeda();

protected:
	void InitConfig();
	bool ConnectBroker();
	void ClearCallback();
    void ClearCedaCallback();
	bool StartServerIfNotRuning();
	bool Reg2Server();
	bool WaitRegSucess();
	void UnReg2Server();
	void StartGuardThread();
	void StopGuardThread();
    t_string GetModuleDirPath();
	//HMODULE GetCurrentModule();

public:
	bool m_bStopGuard;

private:
	engine::zmq::EngineInitializer m_engineInit;
	sdbus::Connection* m_pConn;
	mapMQCallback m_mapCallback;
    mapCedaMQCallback m_mapCedaCallback;
	HANDLE m_hGuardThread;
	DWORD m_dwFindServerProcID;
	DWORD m_dwRegServerProcID;
	DWORD m_dwCurMsgID;
	HANDLE m_hRegSucessEvent;
	CRITICAL_SECTION m_cs;

	ISSMQMngCallback* m_pMngCallback;
    //ss::logger* m_pLogger;
};

CSSMQMng* GetSSMQMngIns();
