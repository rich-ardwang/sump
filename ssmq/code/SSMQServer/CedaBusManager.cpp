#include "stdafx.h"
#include "CedaBusManager.h"



//internal use
CedaBusManager* GetCedaBusMngIns()
{
    static CedaBusManager sRet;
    return &sRet;
}

CedaBusManager::CedaBusManager() : m_pCedaProxy(nullptr)
{
    InitCedaProxy();
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "InitCedaProxy ok.\r\n");
}

CedaBusManager::~CedaBusManager()
{
    if (m_pCedaProxy) {
        delete m_pCedaProxy;
        m_pCedaProxy = nullptr;
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "delete m_pCedaProxy ok.\r\n");
    }
}

CCedaAgent* CedaBusManager::CreateCedaClient(const std::string &usr, const std::string &pwd, std::string &sLoginsid)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientA start, usr=[%s].\r\n", usr.c_str());
    CCedaAgent *pCedaAgt = new CCedaAgent();
    m_pCedaProxy->addClient(pCedaAgt);
    if (pCedaAgt->InitCeda(usr, pwd, sLoginsid)) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientA success.\r\n");
        return pCedaAgt;
    }

    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CedaBusManager::CreateCedaClientA failed!\r\n");
    if (!removeCedaClient(pCedaAgt)) {
        m_pCedaProxy->removeClient(pCedaAgt);
        delete pCedaAgt;
        pCedaAgt = NULL;
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CedaBusManager::CreateCedaClientA, client force removed!\r\n");
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientA end.\r\n");
    return pCedaAgt;
}

CCedaAgent* CedaBusManager::CreateCedaClient(const std::string &loginMsg, std::string &sLoginsid)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientB start, loginMsg=[%s].\r\n", loginMsg.c_str());
    CCedaAgent *pCedaAgt = new CCedaAgent();
    m_pCedaProxy->addClient(pCedaAgt);
    if (pCedaAgt->InitCeda(loginMsg, sLoginsid)) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientB success.\r\n");
        return pCedaAgt;
    }

    /*
    if (!removeCedaClient(pCedaAgt)) {
        m_pCedaProxy->removeClient(pCedaAgt);
        delete pCedaAgt;
        pCedaAgt = NULL;
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CedaBusManager::CreateCedaClientB, client force removed!\r\n");
    }
    */
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientB end.\r\n");
    return pCedaAgt;
}

CCedaAgent* CedaBusManager::CreateCedaClient(const PxyInfo &pxyInfo, std::string &sLoginsid)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientC start, PxyHost=[%s],  \
        PxyPort=[%d], PxyType=[%d], PxyUsr=[%s]\r\n", pxyInfo.sPxyHost.c_str(), pxyInfo.nPxyPort, pxyInfo.nPxyType, pxyInfo.sPxyUsr.c_str());
    CCedaAgent *pCedaAgt = new CCedaAgent();
    m_pCedaProxy->addClient(pCedaAgt);
    if (pCedaAgt->InitCeda(pxyInfo, sLoginsid)) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientC success.\r\n");
        return pCedaAgt;
    }

    if (!removeCedaClient(pCedaAgt)) {
        m_pCedaProxy->removeClient(pCedaAgt);
        delete pCedaAgt;
        pCedaAgt = NULL;
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CedaBusManager::CreateCedaClientC, client force removed!\r\n");
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::CreateCedaClientC end.\r\n");
    return pCedaAgt;
}

bool CedaBusManager::removeCedaClient(CCedaAgent* pCedaAg)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::removeCedaClient start.\r\n");
    if (pCedaAg) {
        if (!pCedaAg->StopCeda()) {
            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CedaBusManager::removeCedaClient, stop ceda failed!\r\n");
            return false;
        }
        m_pCedaProxy->removeClient(pCedaAg);
        delete pCedaAg;
        pCedaAg = NULL;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::removeCedaClient success.\r\n");
    return true;
}

void CedaBusManager::InitCedaProxy()
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::InitCedaProxy start...\r\n");
    m_pCedaProxy = new CedaProxy();
    m_pCedaProxy->setLogLevel(GET_GOL_CONF.stCedaCliInfo.nCedaProxyLogLel);
    m_pCedaProxy->setLogFile(const_cast<char *>(GET_GOL_CONF.stCedaCliInfo.sCedaProxyLogFile.c_str()));
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CedaBusManager::InitCedaProxy end.\r\n");
}