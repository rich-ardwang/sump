#include "StdAfx.h"
#include "SSMQServerFrame.h"
#include "ISSMQDefine.h"
#include <Psapi.h>
#include <process.h>

CSSMQServerFrame::CSSMQServerFrame(void)
:m_hProcMutex(NULL)
,m_bNeedReleaseMutex(false)
,m_bStopGuard(false)
,m_dwProcID(0)
,m_bEnableAddClient(true)
,m_pCedaAgent(nullptr)
,m_pCedaMngReqThread(nullptr)
,m_pCedaSubReqThread(nullptr)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CSSMQServerFrame::CSSMQServerFrame() called.\r\n");
    m_hExitEvent = CreateEvent(NULL, FALSE, FALSE, GetExitEventName().c_str());

    //use for ceda channel
    m_mpCedaMngObs.clear_s();
    m_mpCedaTopicObs.clear_s();
    m_mpTopicsDest.clear_s();
    m_plstCedaDispachThreads.clear();
    MngReqRun();
    SubReqRun();
    DispachMsgRun();
}

CSSMQServerFrame::~CSSMQServerFrame(void)
{
	CloseHandle(m_hExitEvent);
	if (m_bNeedReleaseMutex)
	{
		ReleaseMutex(m_hProcMutex);
	}
	CloseHandle(m_hProcMutex);

    if (m_pCedaMngReqThread) {
        delete m_pCedaMngReqThread;
        m_pCedaMngReqThread = nullptr;
    }

    if (m_pCedaSubReqThread) {
        delete m_pCedaSubReqThread;
        m_pCedaSubReqThread = nullptr;
    }

    if (!m_plstCedaDispachThreads.empty()) {
        ThreadVector::iterator iter = m_plstCedaDispachThreads.begin();
        for (; iter != m_plstCedaDispachThreads.end(); iter++) {
            if (*iter)
                delete (*iter);
                (*iter) = nullptr;
        }
        m_plstCedaDispachThreads.clear();
    }
}

void CSSMQServerFrame::Run()
{
	if (!ProcessSingleton())
	{
		return;
	}
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CSSMQServerFrame::Run() called.\r\n");
	m_dwProcID = ::GetCurrentProcessId();
	InitConfig();
    size_t len = GET_GOL_CONF.sBrokerUrl.length()+1;
    char *pszUrl = new char[len];
    strcpy_s(pszUrl, len, GET_GOL_CONF.sBrokerUrl.c_str());
    engine::zbroker::create_broker(pszUrl, len, GET_GOL_CONF.nMsgQueueSize);
	BeginListenMngMsg();
	StartGuardThread();
	WaitForSingleObject(m_hExitEvent, INFINITE);
    //MngReqJoin();
	StopGuardThread();
    delete[] pszUrl;
}

bool CSSMQServerFrame::NoClientRunning()
{
    boost::mutex::scoped_lock lock(m_mutex);
	for (StringSet::iterator iter = m_setClient.begin(); iter != m_setClient.end(); ++iter)
	{
		DWORD dwProcID = atoi(iter->c_str());
		if (FindProcessByProcID(dwProcID))
		{
			return false;
		}
	}
	return true;
}

void CSSMQServerFrame::InitConfig()
{
	CConfig::getInstance();
}

void CSSMQServerFrame::AddClient(const std::string& sClientName)
{
	boost::mutex::scoped_lock lock(m_mutex);
    if (m_bEnableAddClient)
    {
        m_setClient.insert(sClientName);
    }
}

void CSSMQServerFrame::RemoveClient(const std::string& sClientName)
{
	boost::mutex::scoped_lock lock(m_mutex);
	StringSet::iterator iterf = m_setClient.find(sClientName);
	if (iterf != m_setClient.end())
	{
		m_setClient.erase(iterf);
	}

	if (m_setClient.empty())
	{
		SetEvent(m_hExitEvent);
	}
}

void CSSMQServerFrame::AckProcID(const sdbus::string& sClientPipe)
{
	sdbus::Message msg;
	sdbus::Message::Map& values = msg.MapValue();
	values.SetString(SSMQ_MSGIDX_CMDTYPE, SSMQ_CMD_ACKSRVPROCID);
	values.SetUInt32(SSMQ_MSGIDX_CMDPARA, m_dwProcID);
	m_conn.Send(sClientPipe, msg);
}

void CSSMQServerFrame::CloseAll(const std::string& sCaller)
{
	boost::mutex::scoped_lock lock(m_mutex);
	for (StringSet::iterator iter = m_setClient.begin(); iter != m_setClient.end(); ++iter)
	{
		sdbus::Message msg;
		msg.SetString(SSMQ_MSGIDX_CMDTYPE, SSMQ_CMD_CLOSECLIENT);
		msg.SetString(SSMQ_MSGIDX_CMDPARA, sCaller);
		m_conn.Send(*iter, msg);
	}

	//WaitClientAndClose(sCaller);
}

void CSSMQServerFrame::CloseOneAndReleaseOther(const std::string& sCaller)
{
    boost::mutex::scoped_lock lock(m_mutex);
    m_bEnableAddClient = false;
    m_setClient.clear();
    m_setClient.insert(sCaller);

    sdbus::Message msg;
    msg.SetString(SSMQ_MSGIDX_CMDTYPE, SSMQ_CMD_CLOSECLIENT);
    msg.SetString(SSMQ_MSGIDX_CMDPARA, sCaller);
    m_conn.Send(sCaller, msg);
}

void CSSMQServerFrame::WaitClientAndClose(const std::string& sCaller)
{
	StopGuardThread();

	while (!NoClientRunningButCaller(sCaller))
	{
		int nTimeCnt = 30;
		for (int i = 0; i < nTimeCnt; ++i)
		{
			Sleep(100);
			if (NoClientRunningButCaller(sCaller))
			{
				break;
			}
		}
		break;
	}
	SetEvent(m_hExitEvent);
}

bool CSSMQServerFrame::NoClientRunningButCaller(const std::string& sCaller)
{
	for (StringSet::iterator iter = m_setClient.begin(); iter != m_setClient.end(); ++iter)
	{
		if (*iter == sCaller)
		{
			continue;
		}
		DWORD dwProcID = atoi(iter->c_str());
		if (FindProcessByProcID(dwProcID))
		{
			return false;
		}
	}
	return true;
}

t_string CSSMQServerFrame::GetExitEventName()
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	t_string sPath = szPath;
	SS_BASE::replace_all(sPath, _T("\\"), _T("/"));
	SS_BASE::replace_all(sPath, _T("/"), _T("-"));
	SS_BASE::replace_all(sPath, _T(":"), _T(""));
	return sPath;
}

void CSSMQServerFrame::BeginListenMngMsg()
{
	bool bRet = m_conn.Connect(GET_GOL_CONF.sBrokerUrl, sdbus::string(SSMQ_ENGINE));
	this->Init(SSMQ_TOPIC_MNG, sdbus::DeliverDirect);
	m_conn.AddHandler(this);
}

void CSSMQServerFrame::OnMsgs(const sdbus::Message ** msgs , int count)
{
	const sdbus::Message::Map& values = (*msgs)->MapValue();
	std::string sCmdType;
	values.GetString(SSMQ_MSGIDX_CMDTYPE, sCmdType);
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CSSMQServerFrame::OnMsgs, sCmdType=%s\r\n", sCmdType.c_str());
	if (SSMQ_CMD_ADDCLIENT == sCmdType) {
        std::string sCmdPara;
        values.GetString(SSMQ_MSGIDX_CMDPARA, sCmdPara);
		AddClient(sCmdPara);
		AckProcID(sCmdPara);
	}
	else if (SSMQ_CMD_REMOVECLIENT == sCmdType) {
        std::string sCmdPara;
        values.GetString(SSMQ_MSGIDX_CMDPARA, sCmdPara);
		RemoveClient(sCmdPara);
	}
	else if (SSMQ_CMD_CLOSEALL == sCmdType) {
		//CloseAll(sCmdPara);
        std::string sCmdPara;
        values.GetString(SSMQ_MSGIDX_CMDPARA, sCmdPara);
        CloseOneAndReleaseOther(sCmdPara);
	}
    else if (CEDA_CMD_CEDA_INIT == sCmdType) {
        CedaMngReq *pReq = new CedaMngReq();
        pReq->sCedaCmd = CEDA_CMD_CEDA_INIT;
        values.GetString(SSMQ_MSGIDX_CMDPARA, pReq->stMngObs.sCedaMngKey);
        values.GetString(SSMQ_MSGIDX_CMDPARA+1, pReq->stMngObs.sCedaEventKey);
        m_mngReq.put(pReq);
    }
    else if (CEDA_CMD_LOGIN_WITH_USER == sCmdType) {
        CedaMngReq *pReq = new CedaMngReq();
        pReq->sCedaCmd = CEDA_CMD_LOGIN_WITH_USER;
        values.GetString(SSMQ_MSGIDX_CMDPARA, pReq->stMngObs.sCedaMngKey);
        values.GetString(SSMQ_MSGIDX_CMDPARA+1, pReq->sUsername);
        values.GetString(SSMQ_MSGIDX_CMDPARA+2, pReq->sPwd);
        m_mngReq.put(pReq);
    }
    else if (CEDA_CMD_LOGIN_WITH_JSON == sCmdType) {
        CedaMngReq *pReq = new CedaMngReq();
        pReq->sCedaCmd = CEDA_CMD_LOGIN_WITH_JSON;
        values.GetString(SSMQ_MSGIDX_CMDPARA, pReq->stMngObs.sCedaMngKey);
        values.GetString(SSMQ_MSGIDX_CMDPARA+1, pReq->sValidkey);
        m_mngReq.put(pReq);
    }
    else if (CEDA_CMD_CEDA_SUBSCRIBE == sCmdType) {
        CedaSubscReq *pReq = new CedaSubscReq();
        pReq->sCedaCmd = CEDA_CMD_CEDA_SUBSCRIBE;
        values.GetString(SSMQ_MSGIDX_CMDPARA, pReq->stTopicObs.sTopicObsKey);
        values.GetString(SSMQ_MSGIDX_CMDPARA+1, pReq->stTopicObs.sTopics);
        values.GetString(SSMQ_MSGIDX_CMDPARA+2, pReq->stTopicObs.sSrvID);
        m_subReq.put(pReq);
    }
    else if (CEDA_CMD_CEDA_SEND == sCmdType) {
        //parse para
        std::string sBindKey;
        std::string sTopic;
        std::string sRequest;
        std::string sSrvID;
        values.GetString(SSMQ_MSGIDX_CMDPARA, sBindKey);
        values.GetString(SSMQ_MSGIDX_CMDPARA+1, sTopic);
        values.GetString(SSMQ_MSGIDX_CMDPARA+2, sRequest);
        values.GetString(SSMQ_MSGIDX_CMDPARA+3, sSrvID);
        //check
        if (sBindKey.empty() || sTopic.empty() || sSrvID.empty()) {
            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CEDA_CMD_CEDA_SEND failed! bindKey=[%s],     \
                topic=[%s], request=[%s], srvID=[%s].\r\n", sBindKey.c_str(), sTopic.c_str(), sRequest.c_str(), sSrvID.c_str());
            return;
        }
        if (nullptr == m_pCedaAgent) {
            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "m_pCedaAgent is null!\r\n");
            return;
        }
        //async send to ceda gateway server
        m_pCedaAgent->asyncRequest(const_cast<char *>(sTopic.c_str()), const_cast<char *>(sRequest.c_str()), 
            const_cast<char *>(sSrvID.c_str()), GET_GOL_CONF.stGtwInfo.nGtwIsGZip);
        //add to send queue
        CedaSendReq *pReq = new CedaSendReq();
        pReq->sSendBindkey = sBindKey;
        pReq->sSendTopic = sTopic;
        m_sendReq.put(pReq);
    }
}

bool CSSMQServerFrame::ProcessSingleton()
{
    m_hProcMutex = CreateMutexA(NULL, TRUE, GET_GOL_CONF.sBrokerUrl.c_str());
	if (!m_hProcMutex)
	{
		return false;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return false;
	}
	m_bNeedReleaseMutex = true;
	return true;
}

unsigned int __stdcall GuardThread(PVOID pM)
{
	unsigned int nRet = 0;
	CSSMQServerFrame *pThis = (CSSMQServerFrame*)pM;
	while (!pThis->m_bStopGuard)
	{
		int nTimeCnt = 10;
		for (int i = 0; i < nTimeCnt; ++i)
		{
			Sleep(200);
			if (pThis->m_bStopGuard)
			{
				break;
			}
		}

		if (pThis->NoClientRunning())
		{
			SetEvent(pThis->m_hExitEvent);
			break;
		}
	}

	return nRet;
}

void CSSMQServerFrame::StartGuardThread()
{
	m_bStopGuard = false;
	m_hGuardThread = (HANDLE)_beginthreadex(NULL, 0, GuardThread, this, 0, NULL);
}

void CSSMQServerFrame::StopGuardThread()
{
	m_bStopGuard = true;
	WaitForSingleObject(m_hGuardThread, 1000);
}

BOOL CSSMQServerFrame::FindProcessByProcID(DWORD dwProcID)
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return FALSE;

	cProcesses = cbNeeded / sizeof(DWORD);

	HANDLE hProcess;
	BOOL isRunning = FALSE;
	DWORD curID;
	for (unsigned int i = 0; i < cProcesses; i++)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

		if (NULL != hProcess)
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if (::EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
				curID = ::GetProcessId(hProcess);
				if (curID == dwProcID)
				{
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

boost::thread *CSSMQServerFrame::CreateThreadWorker(void (CSSMQServerFrame::*fn)()) {
    boost::thread* p = new boost::thread(boost::bind(fn, this));
    return p;
}

void CSSMQServerFrame::MngReqRun() {
    m_pCedaMngReqThread = CreateThreadWorker(&CSSMQServerFrame::HandleMngReqTask);
}

void CSSMQServerFrame::MngReqStop()
{
}

void CSSMQServerFrame::MngReqJoin() {
    m_pCedaMngReqThread->join();
}

void CSSMQServerFrame::SubReqRun() {
    m_pCedaSubReqThread = CreateThreadWorker(&CSSMQServerFrame::HandleSubReqTask);
}

void CSSMQServerFrame::SubReqStop()
{
}

void CSSMQServerFrame::SubReqJoin() {
    m_pCedaSubReqThread->join();
}

void CSSMQServerFrame::DispachMsgRun() {
    boost::thread* pTmp = NULL;
    for (int i=0; i<GET_GOL_CONF.stCedaCliInfo.nSubsAckHandleCnt; i++) {
        pTmp = CreateThreadWorker(&CSSMQServerFrame::HandleDispachMsgTask);
        if (pTmp)
            m_plstCedaDispachThreads.push_back(pTmp);
    }
    pTmp = NULL;
}

void CSSMQServerFrame::DispachMsgStop()
{
}

void CSSMQServerFrame::DispachMsgJoin() {
    if (!m_plstCedaDispachThreads.empty()) {
        vector<boost::thread *>::iterator iter = m_plstCedaDispachThreads.begin();
        for (; iter != m_plstCedaDispachThreads.end(); iter++) {
            (*iter)->join();
        }
    }
}

void CSSMQServerFrame::HandleMngReqTask() {
    while (true) {
        CedaMngReq *pReq = m_mngReq.take();
        if (nullptr == pReq) {
            continue;
        }
        CedaMngReq mngReq(*pReq);
        delete pReq;
        pReq = nullptr;
        if (CEDA_CMD_CEDA_INIT == mngReq.sCedaCmd) {
            //parse process id
            std::string pid1;
            pid1 = CUtils::parseProcessID(mngReq.stMngObs.sCedaMngKey, GET_GOL_CONF.stCedaCliInfo.sCedaBindKeySpr);
            if (pid1.empty()) {
                cedaInitAck(mngReq.stMngObs.sCedaMngKey, false);
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "[CEDA_CMD_CEDA_INIT]:parse pid from cedaMngKey failed!\r\n");
                continue;
            }
            //already init
            CedaCliObsMap::iterator iter = m_mpCedaMngObs.find_s(pid1);
            if (iter != m_mpCedaMngObs.end_s()) {
                cedaInitAck(mngReq.stMngObs.sCedaMngKey, true);
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "already init ceda!\r\n");
                continue;
            }
            //accert pid1 == pid2
            std::string pid2;
            pid2 = CUtils::parseProcessID(mngReq.stMngObs.sCedaEventKey, GET_GOL_CONF.stCedaCliInfo.sCedaBindKeySpr);
            if (pid1 != pid2) {
                cedaInitAck(mngReq.stMngObs.sCedaMngKey, false);
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, 
                    "the pids from cedaMngKey and cedaEventKey are different! mngPid=[%s], eventPid=[%s]\r\n", 
                    pid1.c_str(), pid2.c_str());
                continue;
            }
            //reg mngObs
            MngObs mngObs;
            mngObs.sCedaMngKey = mngReq.stMngObs.sCedaMngKey;
            mngObs.sCedaEventKey = mngReq.stMngObs.sCedaEventKey;
            m_mpCedaMngObs.insert_s(pid1, mngObs);
            //init ok
            cedaInitAck(mngObs.sCedaMngKey, true);
            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "init ceda ok.\r\n");
            continue;
        }
        else if (CEDA_CMD_LOGIN_WITH_USER == mngReq.sCedaCmd) {
            //todo
            continue;
        }
        else if (CEDA_CMD_LOGIN_WITH_JSON == mngReq.sCedaCmd) {
            //parse process id
            std::string pid;
            pid = CUtils::parseProcessID(mngReq.stMngObs.sCedaMngKey, GET_GOL_CONF.stCedaCliInfo.sCedaBindKeySpr);
            if (pid.empty()) {
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "[CEDA_CMD_LOGIN_WITH_JSON]:parse pid from cedaMngKey failed!\r\n");
                continue;
            }
            //check pid
            CedaCliObsMap::iterator iter = m_mpCedaMngObs.find_s(pid);
            if (iter == m_mpCedaMngObs.end_s()) {
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "not init ceda!\r\n");
                continue;
            }
            else {
                if (nullptr == m_pCedaAgent) {
                    if (mngReq.sValidkey.empty()) {
                        cedaLoginAck(iter->second.sCedaEventKey, CEDA_LOGIN_INVALID_KEY, "");    //validKey is null
                        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "validKey is null!\r\n");
                        continue;
                    }
                    std::string sid;
                    m_pCedaAgent = GET_CEDA_BUS_MANAGER->CreateCedaClient(mngReq.sValidkey, sid);
                    if (nullptr == m_pCedaAgent) {
                        cedaLoginAck(iter->second.sCedaEventKey, CEDA_LOGIN_FAILED, "");    //ceda login failed
                        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CreateCedaClient failed!\r\n");
                        continue;
                    }
                    else {
                       m_pCedaAgent->setFrame(this);
                       cedaLoginAck(iter->second.sCedaEventKey, CEDA_LOGIN_OK, sid);    //ceda login ok
                       CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "login ceda with json ok.\r\n");
                       continue;
                    }
                }
                else {
                    cedaLoginAck(iter->second.sCedaEventKey, CEDA_LOGIN_OK, m_pCedaAgent->getSid());    //ceda already login, so ok
                    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "already login ceda.\r\n");
                    continue;
                }
            }
        }
    }
}

void CSSMQServerFrame::HandleSubReqTask()
{
    while (true) {
        CedaSubscReq *pReq = m_subReq.take();
        if (nullptr == pReq) {
            continue;
        }
        CedaSubscReq subReq(*pReq);
        delete pReq;
        pReq = nullptr;
        if (CEDA_CMD_CEDA_SUBSCRIBE == subReq.sCedaCmd) {
            //parse pid
            std::string pid;
            pid = CUtils::parseProcessID(subReq.stTopicObs.sTopicObsKey, GET_GOL_CONF.stCedaCliInfo.sCedaBindKeySpr);
            CedaCliObsMap::iterator it = m_mpCedaMngObs.find_s(pid);
            if (it == m_mpCedaMngObs.end_s()) {
                //not reg obs. write log and continue.
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "not init or login ceda!\r\n");
                continue;
            }
            std::string eventObs = (*it).second.sCedaEventKey;
            TopicObsMap::iterator iter = m_mpCedaTopicObs.find_s(subReq.stTopicObs.sTopicObsKey);
            //need add topic obs
            if (iter == m_mpCedaTopicObs.end_s()) {
                std::set<string> setTopics;
                //get topics and remove duplicates
                getTopicSet(subReq.stTopicObs.sTopics, setTopics);
                if (!setTopics.empty()) {
                    m_mpCedaTopicObs.insert_s(subReq.stTopicObs.sTopicObsKey, setTopics);
                    //to do reg subscribes
                    std::set<string>::iterator itTopic = setTopics.begin();
                    for (; itTopic != setTopics.end(); itTopic++) {
                        TopicsDestinationMap::iterator itDes = m_mpTopicsDest.find_s(*itTopic);
                        /*  need add topic desternation
                            subscribe to ceda server
                        */
                        if (itDes == m_mpTopicsDest.end_s()) {
                            std::map<std::string, TopicObs> mpTopicObs;
                            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "m_mpTopicsDest, topicObsKey:[%s]\r\n", subReq.stTopicObs.sTopicObsKey.c_str());
                            mpTopicObs.insert(pair<std::string, TopicObs>(subReq.stTopicObs.sTopicObsKey, subReq.stTopicObs));
                            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "m_mpTopicsDest, topic:[%s]\r\n", (*itTopic).c_str());
                            m_mpTopicsDest.insert_s((*itTopic), mpTopicObs);
                            m_pCedaAgent->subscribe(const_cast<char *>((*itTopic).c_str()), const_cast<char *>(subReq.stTopicObs.sSrvID.c_str()));
                        }
                        else {
                            /*  find topic obs key
                                subscribe local, needn't subscribe to ceda server
                            */
                            std::map<std::string, TopicObs>::iterator itObs = itDes->second.find(subReq.stTopicObs.sTopicObsKey);
                            //need add topic obs key
                            if (itObs == itDes->second.end()) {
                                itDes->second.insert(pair<std::string, TopicObs>(subReq.stTopicObs.sTopicObsKey, subReq.stTopicObs));
                            }
                        }
                    }

                    //return subscribe ok msg to client
                    cedaSubAck(eventObs, CEDA_SUBSCRIBE_OK, "ceda subscribe ok.");
                    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "ceda subscribe ok.\r\n");
                    continue;
                }
                //topic empty
                cedaSubAck(eventObs, CEDA_SUBSCRIBE_TOPIC_EMPTY, "ceda subscribe's topic is empty!");
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "ceda subscribe's topic is empty!\r\n");
                continue;
            }
            //already subscribe topics, needn't subscribe again
            else {
                cedaSubAck(eventObs, CEDA_ALREADY_SUBSCRIBE, "ceda already subscribe!");
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "ceda already subscribe!\r\n");
                continue;
            }
        }
        else if (CEDA_CMD_CEDA_UNSUBSCRIBE == subReq.sCedaCmd) {
        }
    }
}

void CSSMQServerFrame::HandleDispachMsgTask()
{
    while (true) {
        CedaDisMsg *pReq = m_disMsgQue.take();
        if (nullptr == pReq) {
            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CedaDisMsg is null!\r\n");
            continue;
        }
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "dispatch topic=[%s].\r\n", (*pReq).sTopic.c_str());
        //CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "dispatch content=[%s].\r\n", (*pReq).sContent.c_str());
        CedaDisMsg disMsgReq(*pReq);
        delete pReq;
        pReq = nullptr;
        //std::string topic;
        //parse topic
        //topic = CUtils::parseTopicID(disMsgReq.sTopic, GET_GOL_CONF.stCedaCliInfo.sCedaTopicSpr);
        TopicsDestinationMap::iterator iter = m_mpTopicsDest.find_s(disMsgReq.sTopic);
        //describe msg
        if (iter != m_mpTopicsDest.end_s()) {
            //dispatch message to client
            std::map<std::string, TopicObs>::iterator it = iter->second.begin();
            for (; it != iter->second.end(); it++) {
                cedaDispatchMsg(it->first, disMsgReq);
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "dispatch describe msg, topicKey=[%s].\r\n", it->first.c_str());
            }
            continue;
        }
        //async msg and other msg
        else {
            CedaSendReq *pReq = m_sendReq.take();
            if (nullptr == pReq) {
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CedaSendReq is null!\r\n");
                continue;
            }
            CedaSendReq sendReq(*pReq);
            if (CUtils::isInclude(disMsgReq.sTopic, sendReq.sSendTopic)) {
                cedaDispatchMsg(sendReq.sSendBindkey, disMsgReq);
                CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "dispatch async msg, bindKey=[%s].\r\n", sendReq.sSendBindkey.c_str());
                delete pReq;
                pReq = nullptr;
                continue;
            }
            else {
                //not this topic, put into queue
                //at now we can't filter the other messages
                m_sendReq.put(pReq);
                //write log and continue
                //CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "unrecognize topic! topic=[%s].\r\n", disMsgReq.sTopic.c_str());
                continue;
            }
        }
    }
}

sdbus::string CSSMQServerFrame::getCedaBindKey(const sdbus::string &cedaHeader, const sdbus::string &bindKey)
{
    sdbus::string cedaATSKey;
    cedaATSKey += cedaHeader;
    cedaATSKey += GET_GOL_CONF.stCedaCliInfo.sCedaBindKeySpr;
    cedaATSKey += bindKey;
    return cedaATSKey;
}

void CSSMQServerFrame::cedaInitAck(const string &mngBindKey, const bool ret) {
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_CEDA_INIT_ACK);
    values.SetBool(SSMQ_MSGIDX_CMDPARA, ret);
    m_conn.Send(getCedaBindKey(CEDA_ATS_MQ_HEADER, mngBindKey), msg);
}

void CSSMQServerFrame::cedaLoginAck(const string &eventBindKey, const int ret, const std::string sid) {
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_LOGIN_ACK);
    values.SetInt32(SSMQ_MSGIDX_CMDPARA, ret);
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, sid);
    m_conn.Send(getCedaBindKey(CEDA_ATS_MQ_HEADER, eventBindKey), msg);
}

void CSSMQServerFrame::getTopicSet(const std::string &topics, std::set<string> &topicSet)
{
    vector<string> vTopic;
    CUtils::split(topics, '|', vTopic);
    if (!vTopic.empty()) {
        vector<string>::iterator it = vTopic.begin();
        for (; it != vTopic.end(); it++) {
            topicSet.insert(*it);
        }
    }
}

void CSSMQServerFrame::cedaSubAck(const string &eventBindKey, const int ret, const std::string &retMsg)
{
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_CMD_CEDA_SUBSCRIBE_ACK);
    values.SetInt32(SSMQ_MSGIDX_CMDPARA, ret);
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, retMsg);
    m_conn.Send(getCedaBindKey(CEDA_ATS_MQ_HEADER, eventBindKey), msg);
}

void CSSMQServerFrame::cedaDispatchMsg(const string &topicBindKey, const CedaDisMsg &disMsg)
{
    sdbus::Message msg;
    sdbus::Message::Map& values = msg.MapValue();
    values.SetString(SSMQ_MSGIDX_CMDTYPE, CEDA_DISPATCH_RES_MSG);
    values.SetString(SSMQ_MSGIDX_CMDPARA, disMsg.sTopic);
    values.SetString(SSMQ_MSGIDX_CMDPARA+1, disMsg.sContent);
    values.SetInt32(SSMQ_MSGIDX_CMDPARA+2, disMsg.nLen);
    msg.SetMessageID(getCedaBindKey(CEDA_ATS_MQ_HEADER, topicBindKey));
    m_conn.Send(getCedaBindKey(CEDA_ATS_MQ_HEADER, topicBindKey), msg);
}

void CSSMQServerFrame::putCedaDispatch(CedaDisMsg *pDis)
{
    m_disMsgQue.put(pDis);
}
