#include "StdAfx.h"
#include "SSMQMng.h"
#include "MQCallback.h"
#include "ISSMQDefine.h"
#include "MyCSLock.h"
#include <Psapi.h>
#include <process.h>
#include <io.h>
#include "SSRTL\SSLog.h"
#include "LogMarco.h"

#define  UNIQUE_EVENTNAME_RGSUCESS "CBCCC676-0E05-4f3f-9F50-8947142F4FAC"

extern HMODULE AfxGetCurrentModule();

//internal use
CSSMQMng* GetSSMQMngIns()
{
    static CSSMQMng sRet;
    return &sRet;
}

//external use
SSMQ::ISSMQMng* GetSSMQMng()
{
    return GetSSMQMngIns();
}

CSSMQMng::CSSMQMng(void)
:m_bStopGuard(false)
,m_dwFindServerProcID(0)
,m_dwRegServerProcID(0)
,m_dwCurMsgID(0)
,m_hRegSucessEvent(NULL)
,m_pConn(NULL)
,m_pMngCallback(NULL)
{
	InitConfig();
	InitializeCriticalSection(&m_cs);

    //m_pLogger = new ss::logger("log\\SSMQ");
    //m_pLogger->set_level_by_config("config\\logconfig.ini", "maincfg.ssmq");
}

CSSMQMng::~CSSMQMng(void)
{
	if (m_pConn)
	{
		delete m_pConn;
		m_pConn = NULL;
	}

	if (m_hRegSucessEvent)
	{
		CloseHandle(m_hRegSucessEvent);
		m_hRegSucessEvent = NULL;
	}
	DeleteCriticalSection(&m_cs);

    /*
    if (m_pLogger)
    {
        delete m_pLogger;
        m_pLogger = NULL;
    }
    */
}

bool CSSMQMng::InitServer()
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin initServer!\r\n");
	bool bSvrReady = StartServerIfNotRuning();
	bool bRet = ConnectBroker();
	//Reg2Server之前先加入监听器
	if (!bRet)
	{
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "InitServer Failed!\r\n");
		return false;
	}
	RegObs(this);
	bRet = Reg2Server();
	if (!bRet)
	{
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "InitServer Failed!\r\n");
		return false;
	}
	if (bSvrReady)
	{
		StartGuardThread();
	}

    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "InitServer Success!\r\n");
	return bRet;
}

bool CSSMQMng::ReleaseServer()
{
	CMyCSLock lk(&m_cs);
	UnReg2Server();
	ClearCallback();
    UnRegCeda2Server();
    ClearCedaCallback();
	StopGuardThread();
	if (m_pConn)
	{
		m_pConn->Close();
		delete m_pConn;
		m_pConn = NULL;
	}
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "ReleaseServer Success!\r\n");
	return true;
}

void CSSMQMng::SetMngCallback(ISSMQMngCallback* pCallback)
{
	m_pMngCallback = pCallback;
}

void CSSMQMng::CloseLocalDataBus()
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "CloseLocalDataBus!\r\n");
	sdbus::Message msg;
	msg.SetString(SSMQ_MSGIDX_CMDTYPE, SSMQ_CMD_CLOSEALL);
	msg.SetString(SSMQ_MSGIDX_CMDPARA, GetClientName());
	Post(SSMQ_TOPIC_MNG, msg);
}

bool CSSMQMng::RegObs(ISSMQObs* pObs)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin RegObs %p!\r\n", pObs);
	CMQCallback* pCallBack = new(std::nothrow) CMQCallback();
	if (!pCallBack)
	{
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "RegObs Failed!\r\n");
		return false;
	}

	pCallBack->InitBySSMQObs(pObs);
	if (!m_pConn)
	{
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "RegObs Failed!\r\n");
		return false;
	}
	m_pConn->AddHandler(pCallBack);
	m_mapCallback[pObs->GetBindKey()] = pCallBack;
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "RegObs Success!\r\n");
	return true;
}

void CSSMQMng::ReleaseObs(ISSMQObs* pObs)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin ReleaseObs %p!\r\n", pObs);
    mapMQCallback::iterator iterf = m_mapCallback.find(pObs->GetBindKey());
    if (iterf != m_mapCallback.end())
    {
        if (m_pConn)
        {
            m_pConn->DelHandler(iterf->second);
        }
        
        delete iterf->second;
        m_mapCallback.erase(iterf);
    }
}

void CSSMQMng::ReleaseAllObs()
{
    ClearCallback();
    ClearCedaCallback();
}

bool CSSMQMng::Send(const sdbus::string& bindKey , const sdbus::Message& request , sdbus::Message& response , int timeout/* = -1*/)
{
	CMyCSLock lk(&m_cs);
	if (!m_pConn)
	{
        ReConnectServer();
		return false;
	}
	bool bRet = m_pConn->Send(bindKey, request, response, timeout);
	if (!bRet)
	{
		ReConnectServer();
	}
	return bRet;
}

bool CSSMQMng::Post(const sdbus::string& bindKey , const sdbus::Message& msg)
{
	CMyCSLock lk(&m_cs);
	if (!m_pConn)
	{
        ReConnectServer();
		return false;
	}
	bool bRet = m_pConn->Send(bindKey, msg);
	if (!bRet)
	{
		ReConnectServer();
	}
	return bRet;
}

bool CSSMQMng::Publish(const sdbus::string& bindKey , const sdbus::Message& msg)
{
	CMyCSLock lk(&m_cs);
	if (!m_pConn)
	{
        ReConnectServer();
		return false;
	}
	bool bRet = m_pConn->Publish(bindKey, msg);
	if (!bRet)
	{
		ReConnectServer();
	}
	return bRet;
}

sdbus::string CSSMQMng::GetUniqueMsgID()
{
	m_dwCurMsgID++;

	char buf[32];
    memset(buf, 0, 32);
	DWORD dwPid = GetCurrentProcessId();
	sprintf_s(buf,32, "%d_%d", dwPid, m_dwCurMsgID);
	return buf;
}

bool CSSMQMng::isConnectBroker()
{
    bool bRet = false;
    if (m_pConn) {
        bRet = true;
    }
    return bRet;
}

sdbus::string CSSMQMng::GetBindKey()
{
	return GetClientName();
}

SSMQDeliverType CSSMQMng::GetDeliverType()
{
	return SSMQDT_Direct;
}

void CSSMQMng::OnMsgs(const sdbus::Message& msgs)
{
	sdbus::string sCmdType;
	msgs.GetString(SSMQ_MSGIDX_CMDTYPE, sCmdType);
	if (sCmdType == SSMQ_CMD_ACKSRVPROCID)
	{
		msgs.GetUInt32(SSMQ_MSGIDX_CMDPARA, (sdbus::uint32_t &)m_dwRegServerProcID);
		SetEvent(m_hRegSucessEvent);
	}
	else if (sCmdType == SSMQ_CMD_CLOSECLIENT)
	{
		std::string sClientName("");
		msgs.GetString(SSMQ_MSGIDX_CMDPARA, sClientName);
		if (sClientName == GetClientName())
		{
			//ReleaseServer();
		}
		if (m_pMngCallback)
		{
			m_pMngCallback->CloseClient();
		}
	}
}

sdbus::string CSSMQMng::GetClientName()
{
	//进程id作为ClientName
	char buf[32];
	DWORD dwPid = GetCurrentProcessId();
	sprintf_s(buf,32, "%d", dwPid);
	return buf;
}

BOOL CSSMQMng::FindProcessByName(LPCTSTR szFileName)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "FindProcessByName!\r\n");
	/************************************************************************/
    // 采用直接枚举进程的方法查找指定名称进程
    /* 名    称: FindProcessByName( LPCTSTR szFileName )
    /* 功    能: 查询指定名称的进程是否有实例正在运行
    /* 参　　数: LPCTSTR szFileName, 可执行程序名称, 例如"c:\\notepad.exe"
    /* 返 回 值: BOOL.TRUE, 有同名实例正在运行; FALSE, 没有同名实例正在运行.
    /* 编 写 者: Xianren Li
    /* 创建日期: 2007-08-17
    /* 附加说明: 请注意进程的查找只是依据名称, 无法确保找到的进程调用的是预料
    /*             中的可执行文件.
    /* 修改记录:
    /************************************************************************/
	t_string sFileName(szFileName);

	DWORD aProcesses[1024], cbNeeded, cProcesses;

    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "EnumProcesses!\r\n");
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return FALSE;

	cProcesses = cbNeeded / sizeof(DWORD);

	TCHAR szProcessName[MAX_PATH] = _T("<unknown>");
	HANDLE hProcess;
	t_string strName;
	BOOL isRunning = FALSE;
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Processes count = %d!\r\n", cProcesses);
	for (unsigned int i = 0; i < cProcesses; i++)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

		if (NULL != hProcess)
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
				::GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
				strName = szProcessName;
				if (SS_BASE::CompareNoCase(sFileName, strName) == 0)
				{
					m_dwFindServerProcID = ::GetProcessId(hProcess);
					isRunning = TRUE;
					CloseHandle(hProcess);
					break;
				}
			}
			CloseHandle(hProcess);
		}
	}
	return isRunning;
}

bool CSSMQMng::IsConnectRightSrv()
{
	return m_dwFindServerProcID == m_dwRegServerProcID;
}

void CSSMQMng::RestartServer()
{
	CMyCSLock lk(&m_cs);
	StartServerIfNotRuning();
	ReConnectServer();
}

void CSSMQMng::ReConnectServer()
{
	CMyCSLock lk(&m_cs);
	ConnectBroker();
	if (!m_pConn)
	{
		return;
	}
	//重新添加监听回调
	for (mapMQCallback::iterator iter = m_mapCallback.begin(); iter != m_mapCallback.end(); ++iter)
	{
		m_pConn->AddHandler(iter->second);
	}
	Reg2Server();

    //重置Ceda的监听回调
    if (GetCedaATSMQMngIns()->IsCedaInit() &&
        (!m_mapCedaCallback.empty())) {
        for (mapCedaMQCallback::iterator iter = m_mapCedaCallback.begin(); iter != m_mapCedaCallback.end(); ++iter)
            m_pConn->AddHandler(iter->second);
        //GetCedaATSMQMngIns()->RegCeda2Server();
    }
}

bool CSSMQMng::RegCedaObs(ISSMQObs* pObs, const sdbus::string &cedaHeader)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin reg ceda obs %p...\r\n", pObs);
    CMQCallback* pCallBack = new(std::nothrow) CMQCallback();
    if (!pCallBack) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "Reg ceda obs failed!\r\n");
        return false;
    }
    pCallBack->InitByCedaATSMQObs(pObs, cedaHeader);

    if (!m_pConn) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "Reg ceda obs failed!\r\n");
        return false;
    }
    m_pConn->AddHandler(pCallBack);
    sdbus::string cedaBindKey = GetCedaATSMQMngIns()->getCedaBindKey(cedaHeader, pObs->GetBindKey());
    m_mapCedaCallback[cedaBindKey] = pCallBack;
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Reg ceda obs success.\r\n");
    return true;
}

void CSSMQMng::UnRegCeda2Server()
{
    if (m_mapCedaCallback.empty())
        return;
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin UnRegCeda2Server...\r\n");
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_UNREG_TO_SERVER);
    values.SetString(SSMQ_MSGIDX_CMDPARA, GetCedaATSMQMngIns()->GetCedaClientName());
    if (Post(SSMQ_TOPIC_MNG, msg)) {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "UnRegCeda2Server failed!\r\n");
    }
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "UnRegCeda2Server finish.\r\n");
}

void CSSMQMng::ReleaseCedaObs(ISSMQObs* pObs, const sdbus::string &cedaHeader)
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin ReleaseCedaObs %p...\r\n", pObs);
    mapMQCallback::iterator iterf = m_mapCedaCallback.find(GetCedaATSMQMngIns()->getCedaBindKey(cedaHeader, pObs->GetBindKey()));
    if (iterf != m_mapCedaCallback.end()) {
        if (m_pConn)
            m_pConn->DelHandler(iterf->second);
        delete iterf->second;
        m_mapCedaCallback.erase(iterf);
    }
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "ReleaseCedaObs success.\r\n");
}

void CSSMQMng::ReleaseAllCedaObs()
{
    ClearCedaCallback();
}

bool CSSMQMng::ReleaseCeda()
{
    CMyCSLock lk(&m_cs);
    UnRegCeda2Server();
    ClearCedaCallback();
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "ReleaseCeda success.\r\n");
    return true;
}

void CSSMQMng::InitConfig()
{
    CConfig::getInstance();
}

bool CSSMQMng::ConnectBroker()
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin ConnectBroker!\r\n");
	CMyCSLock lk(&m_cs);
	if (m_pConn)
	{
		m_pConn->Close();
		delete m_pConn;
		m_pConn = NULL;
	}
	m_pConn = new sdbus::Connection();
    bool bRet = m_pConn->Connect(GET_GOL_CONF.sBrokerUrl ,sdbus::string(SSMQ_ENGINE));
    if (bRet)
    {
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "ConnectBroker Success!\r\n");
    }
    else
    {
        if (m_pConn)
        {
            delete m_pConn;
            m_pConn = NULL;
        }
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "ConnectBroker Failed!\r\n");
    }
	return bRet;
}

void CSSMQMng::ClearCallback()
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "ClearAllCallback!\r\n");
	for (mapMQCallback::iterator iter = m_mapCallback.begin(); iter != m_mapCallback.end(); ++iter)
	{
		if (iter->second)
		{
			m_pConn->DelHandler(iter->second);
			delete (iter->second);
			(iter->second) = NULL;
		}
	}
	m_mapCallback.clear();
}

void CSSMQMng::ClearCedaCallback()
{
    if (m_mapCedaCallback.empty())
        return;
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "ClearAllCedaCallback begin...\r\n");
    for (mapCedaMQCallback::iterator iter = m_mapCedaCallback.begin(); iter != m_mapCedaCallback.end(); ++iter) {
        if (iter->second) {
            m_pConn->DelHandler(iter->second);
            delete (iter->second);
            (iter->second) = NULL;
        }
    }
    m_mapCedaCallback.clear();
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "ClearAllCedaCallback finish.\r\n");
}

bool CSSMQMng::StartServerIfNotRuning()
{
	if (FindProcessByName(SSMQ_SRVNAME))
	{
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Server is running!\r\n");
		return true;
	}

    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Not find server, begin start server!\r\n");

	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	t_string sFullPath(_T("\""));
	sFullPath += GetModuleDirPath();
	sFullPath += _T("SSMQServer.exe");
	sFullPath += _T("\"");

	std::string sChkPath = SS_BASE::t_string2string(sFullPath);
	sChkPath = sChkPath.substr(1, sChkPath.length() - 2);
	if ((_access(sChkPath.c_str(), 0)) == -1)
	{
		return false;
	}

	TCHAR xhFullPath[MAX_PATH] = {_T('\0')};
	for (size_t i = 0; i< sFullPath.size(); ++i)
	{
		xhFullPath[i] = sFullPath[i];
	}
	CreateProcess(NULL, xhFullPath, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	//CreateProcess(NULL, xhFullPath, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	Sleep(100);
	return true;
}

bool CSSMQMng::Reg2Server()
{
    CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_INFO, "Begin Reg To Server!\r\n");
	m_hRegSucessEvent = CreateEventA(NULL, FALSE, FALSE, UNIQUE_EVENTNAME_RGSUCESS);
	sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(SSMQ_MSGIDX_CMDTYPE, SSMQ_CMD_ADDCLIENT);
	values.SetString(SSMQ_MSGIDX_CMDPARA, GetClientName());
	if (!Post(SSMQ_TOPIC_MNG, msg))
	{
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "Reg To Server Failed!\r\n");
	}
	return WaitRegSucess();
}

bool CSSMQMng::WaitRegSucess()
{
	if (WAIT_TIMEOUT == WaitForSingleObject(m_hRegSucessEvent, /*INFINITE*/5000))
	{
        CURLOGGERWRITEWITHLV(GetCedaCliLogger(), ss::LOG_LEVEL::LOG_ERR, "Wait Reg Ack Timeout!\r\n");
		return false;
	}
	return true;
}

void CSSMQMng::UnReg2Server()
{
	sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(SSMQ_MSGIDX_CMDTYPE, SSMQ_CMD_REMOVECLIENT);
	values.SetString(SSMQ_MSGIDX_CMDPARA, GetClientName());
	Post(SSMQ_TOPIC_MNG, msg);
}

unsigned int __stdcall GuardThread(PVOID pM)
{
	unsigned int nRet = 0;
	CSSMQMng *pThis = (CSSMQMng*)pM;

	while (!pThis->m_bStopGuard)
	{
		if (!pThis->FindProcessByName(SSMQ_SRVNAME))
		{
			pThis->RestartServer();
			continue;
		}
		else if (!pThis->IsConnectRightSrv())
		{
			pThis->ReConnectServer();
		}
		int nTimeCnt = 4;
		for (int i = 0; i < nTimeCnt; ++i)
		{
			Sleep(500);
			if (pThis->m_bStopGuard)
			{
				break;
			}
		}
	}

	return nRet;
}

void CSSMQMng::StartGuardThread()
{
	m_bStopGuard = false;
	m_hGuardThread = (HANDLE)_beginthreadex(NULL, 0, GuardThread, this, 0, NULL);
}

void CSSMQMng::StopGuardThread()
{
	m_bStopGuard = true;
	WaitForSingleObject(m_hGuardThread, 1000);
}

t_string CSSMQMng::GetModuleDirPath()
{
	TCHAR szFileNames[260];
	DWORD dwLen = GetModuleFileName(AfxGetCurrentModule(), szFileNames, sizeof(szFileNames));
	for(DWORD offset=dwLen; offset>=0; offset--)
	{
		if(szFileNames[offset] == _T('\\'))
		{
			szFileNames[offset] = _T('\0');
			break;
		}
	}

	t_string sRet = szFileNames;
	sRet += _T("\\");
	return sRet;
}

/*
HMODULE CSSMQMng::GetCurrentModule()
{

#if _MSC_VER < 1300    // earlier than .NET compiler (VC 6.0)

	// Here's a trick that will get you the handle of the module

	// you're running in without any a-priori knowledge:

	MEMORY_BASIC_INFORMATION mbi;

	static int dummy;

	VirtualQuery( &dummy, &mbi, sizeof(mbi) );

	return reinterpret_cast<HMODULE>(mbi.AllocationBase);

#else  

	// from ATL 7.0 sources

	return reinterpret_cast<HMODULE>(&__ImageBase);

#endif
} 
*/
