#include "stdafx.h"
#include "CedaAgent.h"


CCedaAgent::CCedaAgent() : m_cedaInit(false),
                           m_cedaLoginSig(0),
                           m_cedaStopSig(0),
                           m_bLoginTimeout(false),
                           m_bStopTimeout(false)
{
}

CCedaAgent::~CCedaAgent()
{
}

bool CCedaAgent::InitCeda(const std::string &usr, const std::string &pwd)
{
    return true;
}

bool CCedaAgent::InitCeda(const std::string &loginMsg)
{
    if (m_cedaInit) {
        cout << "need not init ceda." << endl;
        return true;
    }

    setLoginMessage(const_cast<char *>(loginMsg.c_str()));
    ConnectCedaMQ();
    boost::thread t(&CCedaAgent::loginTimeout, this);
    m_cedaLoginSig.wait();
    if (m_bLoginTimeout) {
        m_bLoginTimeout = false;
        cout << "init ceda timeout." << endl;
        return false;
    }
    cout << "init ceda success." << endl;
    return true;
}

bool CCedaAgent::InitCeda(const PxyInfo &pxyInfo)
{
    return true;
}

bool CCedaAgent::StopCeda()
{
    stop();
    boost::thread t(&CCedaAgent::stopTimeout, this);
    m_cedaStopSig.wait();
    if (m_bLoginTimeout) {
        m_bLoginTimeout = false;
        cout << "stop ceda timeout!" << endl;
        return false;
    }
    cout << "stop ceda success." << endl;
    return true;
}

bool CCedaAgent::isInitCeda()
{
    return m_cedaInit;
}

void CCedaAgent::onMessage(const char* topic, const char *content, int len)
{
    string sRecvTopic(topic);
    string sContent(content);
    cout << "recvTopic=[" << sRecvTopic << "]" << endl;
    cout << "content=[" << sContent << "]" << endl;
    cout << "len=[" << len << "]" << endl;
}

void CCedaAgent::onEvent(int nevent)
{
    switch (nevent) {
    case CONNECTION_LOGIN_SUCCESS:
        m_cedaInit = true;
        m_cedaLoginSig.signal();
        break;
    case CONNECTION_CLOSED:
        m_cedaInit = false;
        m_cedaStopSig.signal();
		break;
    default:
        break;
    }
}

void CCedaAgent::ConnectCedaMQ()
{
    connect("172.16.97.106", 9060, 1);
}

void CCedaAgent::loginTimeout()
{
    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    m_bLoginTimeout = true;
    m_cedaLoginSig.signal();
}

void CCedaAgent::stopTimeout()
{
    boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
    m_bStopTimeout = true;
    m_cedaStopSig.signal();
}