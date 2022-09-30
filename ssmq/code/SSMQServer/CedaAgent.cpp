#include "stdafx.h"
#include "CedaAgent.h"



CCedaAgent::CCedaAgent() : m_cedaInit(false),
                           m_cedaLoginSig(0),
                           m_cedaStopSig(0),
                           m_bLoginTimeout(false),
                           m_bStopTimeout(false),
                           m_strLoginSid(""),
                           m_pFrame(nullptr)
{
}

CCedaAgent::~CCedaAgent()
{
}

bool CCedaAgent::InitCeda(const std::string &usr, const std::string &pwd, std::string &sLoginsid)
{
    if (m_cedaInit) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const std::string &usr, const std::string &pwd), needn't init ceda.\r\n");
        sLoginsid = m_strLoginSid;
        return true;
    }

    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const std::string &usr, const std::string &pwd), init ceda begin...\r\n");
    setUserPass(const_cast<char *>(usr.c_str()), const_cast<char *>(pwd.c_str()));
    ConnectCedaMQ();
    boost::thread t(&CCedaAgent::loginTimeout, this);
    m_cedaLoginSig.wait();
    if (m_bLoginTimeout) {
        m_bLoginTimeout = false;
        sLoginsid = "";
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CCedaAgent::InitCeda(const std::string &usr, const std::string &pwd), init ceda timeout!\r\n");
        return false;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const std::string &usr, const std::string &pwd), init ceda success.\r\n");
    sLoginsid = m_strLoginSid;
    return true;
}

bool CCedaAgent::InitCeda(const std::string &loginMsg, std::string &sLoginsid)
{
    if (m_cedaInit) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const std::string &loginMsg), needn't init ceda.\r\n");
        sLoginsid = m_strLoginSid;
        return true;
    }

    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const std::string &loginMsg), init ceda begin...\r\n");
    setLoginMessage(const_cast<char *>(loginMsg.c_str()));
    ConnectCedaMQ();
    boost::thread t(&CCedaAgent::loginTimeout, this);
    m_cedaLoginSig.wait();
    if (m_bLoginTimeout) {
        m_bLoginTimeout = false;
        sLoginsid = "";
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CCedaAgent::InitCeda(const std::string &loginMsg), init ceda timeout!\r\n");
        return false;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const std::string &loginMsg), init ceda success.\r\n");
    sLoginsid = m_strLoginSid;
    return true;
}

bool CCedaAgent::InitCeda(const PxyInfo &pxyInfo, std::string &sLoginsid)
{
    if (m_cedaInit) {
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const PxyInfo &pxyInfo), needn't init ceda.\r\n");
        sLoginsid = m_strLoginSid;
        return true;
    }

    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const PxyInfo &pxyInfo), init ceda begin...\r\n");
    setProxyInfo(pxyInfo.nPxyType, const_cast<char *>(pxyInfo.sPxyHost.c_str()), pxyInfo.nPxyPort,
        const_cast<char *>(pxyInfo.sPxyPwd.c_str()), const_cast<char *>(pxyInfo.sPxyPwd.c_str()));
    ConnectCedaMQ();
    boost::thread t(&CCedaAgent::loginTimeout, this);
    m_cedaLoginSig.wait();
    if (m_bLoginTimeout) {
        m_bLoginTimeout = false;
        sLoginsid = "";
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CCedaAgent::InitCeda(const PxyInfo &pxyInfo), init ceda timeout!\r\n");
        return false;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::InitCeda(const PxyInfo &pxyInfo), init ceda success.\r\n");
    sLoginsid = m_strLoginSid;
    return true;
}

bool CCedaAgent::StopCeda()
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::StopCeda(), stop ceda begin...\r\n");
    stop();
    boost::thread t(&CCedaAgent::stopTimeout, this);
    m_cedaStopSig.wait();
    if (m_bLoginTimeout) {
        m_bLoginTimeout = false;
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "CCedaAgent::StopCeda(), stop ceda timeout!\r\n");
        return false;
    }
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::StopCeda(), stop ceda success.\r\n");
    return true;
}

bool CCedaAgent::isInitCeda()
{
    return m_cedaInit;
}

std::string CCedaAgent::getSid()
{
    return m_strLoginSid;
}

void CCedaAgent::setFrame(CSSMQServerFrame* pFrame)
{
    m_pFrame = pFrame;
}

void CCedaAgent::onMessage(const char* topic, const char* content, int len)
{
    string sRecvTopic(topic);
    //Login msg
    if (0 == sRecvTopic.compare(CEDA_LOGIN_RECV_TOPIC)) {
        m_strLoginSid = parseJson(content, "sid");
        m_cedaInit = true;
        m_cedaLoginSig.signal();
    }
    //Subscribe or asyncRequest return msg
    else {
        if (nullptr == m_pFrame) {
            CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_WARN, "m_pFrame is null!\r\n");
            return;
        }
        string sRecvContent(content);
        CedaDisMsg *pDisMsg = new CedaDisMsg();
        pDisMsg->sTopic = sRecvTopic;
        pDisMsg->sContent = sRecvContent;
        pDisMsg->nLen = len;
        m_pFrame->putCedaDispatch(pDisMsg);
    }
}

void CCedaAgent::onEvent(int nevent)
{
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::onEvent(int nevent), event=%d\r\n", nevent);
    switch (nevent) {
    case CONNECTION_LOGIN_SUCCESS:
        break;
    case CONNECTION_CLOSED:
        m_cedaInit = false;
        m_cedaStopSig.signal();
        //ÖØÐÂ¶©ÔÄ
        break;
    default:
        break;
    }
}

void CCedaAgent::ConnectCedaMQ()
{
    connect(const_cast<char *>(GET_GOL_CONF.stGtwInfo.sGtwHost.c_str()), GET_GOL_CONF.stGtwInfo.nGtwPort,
        GET_GOL_CONF.stGtwInfo.nGtwProtocol);
    CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::ConnectCedaMQ(), connect done.\r\n");
}

void CCedaAgent::loginTimeout()
{
    boost::this_thread::sleep(boost::posix_time::milliseconds(GET_GOL_CONF.stCedaCliInfo.nCedaLoginTimeout));
    m_bLoginTimeout = true;
    m_cedaLoginSig.signal();
}

void CCedaAgent::stopTimeout()
{
    boost::this_thread::sleep(boost::posix_time::milliseconds(GET_GOL_CONF.stCedaCliInfo.nCedaStopTimeout));
    m_bStopTimeout = true;
    m_cedaStopSig.signal();
}

std::string CCedaAgent::parseJson(const char* content, const char *sFiled) {
    std::string sRet;
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
        CURLOGGERWRITEWITHLV(GetSrvLogger(), ss::LOG_LEVEL::LOG_DEBUG, "CCedaAgent::parseJson, parseJson failed!\r\n");
    }
    return sRet;
}
