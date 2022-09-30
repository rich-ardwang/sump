#include "stdafx.h"
#include "CedaBusManager.h"


CedaBusManager::CedaBusManager() : m_pCedaProxy(nullptr)
{
    InitCedaProxy();
    cout << "InitCedaProxy" << endl;
}

CedaBusManager::~CedaBusManager()
{
    if (m_pCedaProxy) {
        delete m_pCedaProxy;
        m_pCedaProxy = nullptr;
    }
}

CCedaAgent* CedaBusManager::CreateCedaClient(const std::string &usr, const std::string &pwd)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientA start, usr=[%s].\r\n", usr.c_str());
    CCedaAgent *pCedaAgt = new CCedaAgent();
    m_pCedaProxy->addClient(pCedaAgt);
    if (pCedaAgt->InitCeda(usr, pwd)) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientA success.\r\n");
        return pCedaAgt;
    }

    if (!removeCedaClient(pCedaAgt)) {
        m_pCedaProxy->removeClient(pCedaAgt);
        delete pCedaAgt;
        pCedaAgt = NULL;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientA end.\r\n");
    return pCedaAgt;
}

CCedaAgent* CedaBusManager::CreateCedaClient(const std::string &loginMsg)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientB start, loginMsg=[%s].\r\n", loginMsg.c_str());
    CCedaAgent *pCedaAgt = new CCedaAgent();
    m_pCedaProxy->addClient(pCedaAgt);
    if (pCedaAgt->InitCeda(loginMsg)) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientB success.\r\n");
        return pCedaAgt;
    }

    if (!removeCedaClient(pCedaAgt)) {
        //m_pCedaProxy->removeClient(pCedaAgt);
        delete pCedaAgt;
        pCedaAgt = NULL;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientB end.\r\n");
    return pCedaAgt;
}

CCedaAgent* CedaBusManager::CreateCedaClient(const PxyInfo &pxyInfo)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientC start, PxyHost=[%s],  \
        PxyPort=[%d], PxyType=[%d], PxyUsr=[%s]\r\n", pxyInfo.sPxyHost.c_str(), pxyInfo.nPxyPort, pxyInfo.nPxyType, pxyInfo.sPxyUsr.c_str());
    CCedaAgent *pCedaAgt = new CCedaAgent();
    m_pCedaProxy->addClient(pCedaAgt);
    if (pCedaAgt->InitCeda(pxyInfo)) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientC success.\r\n");
        return pCedaAgt;
    }

    if (!removeCedaClient(pCedaAgt)) {
        m_pCedaProxy->removeClient(pCedaAgt);
        delete pCedaAgt;
        pCedaAgt = NULL;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientC end.\r\n");
    return pCedaAgt;
}

bool CedaBusManager::removeCedaClient(CCedaAgent* pCedaAg)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::removeCedaClient start.\r\n");
    if (pCedaAg) {
        if (!pCedaAg->StopCeda()) {
            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_ERR, "CedaBusManager::removeCedaClient failed!\r\n");
            return false;
        }
        //CedaApi移除client后会出现程序崩溃，所以只能单客户端使用，或者干脆不移除。
        /*
        m_pCedaProxy->removeClient(pCedaAg);
        delete pCedaAg;
        pCedaAg = NULL;
        */
    }
    cout << "delete ok" << endl;
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::removeCedaClient success.\r\n");
    return true;
}

void CedaBusManager::InitCedaProxy()
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::InitCedaProxy start.\r\n");
    m_pCedaProxy = new CedaProxy();
    m_pCedaProxy->setLogLevel(3);
    m_pCedaProxy->setLogFile(".\\CedaATSMQClient");
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::InitCedaProxy end.\r\n");
}