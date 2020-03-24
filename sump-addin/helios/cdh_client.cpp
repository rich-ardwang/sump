#include "stdafx.h"
#include "cdh_client.h"
#include "lava_utils_api.h"

void CDHClient::CDHListener::onDisConnected()
{
    log_error(helios_module, "Connection to gateway disConnected!");
}

void CDHClient::CDHListener::onAuthMessage(AuthMessage* pAuthMsg, int len)
{
    if (nullptr == pAuthMsg)
    {
        log_warn(helios_module, "Auth message is null!");
        return;
    }
    for (int i = 0; i < len; ++i)
    {
        log_info(helios_module, "Auth info catgeory: %s, subCategory: %s, sourceIds: %s.",
            pAuthMsg[i].category, pAuthMsg[i].subCategory, pAuthMsg[i].sourceIds);
    }
}

void CDHClient::CDHListener::onKickOut()
{
    log_error(helios_module, "kick out by gateway!");
}

CDHClient::~CDHClient()
{
    if (m_pGtwConnector)
    {
        if (m_pToken)
            //m_pGtwConnector->qbLogout(m_pToken);
            int placeholder = 0;

        delete m_pGtwConnector;
        m_pGtwConnector = nullptr;

        if (m_pListener)
        {
            delete m_pListener;
            m_pListener = nullptr;
        }
    }
}

bool CDHClient::Connect(const std::string &ip, const int &port)
{
    if (m_bConnected)
        return true;

    if (nullptr == m_pGtwConnector)
    {
        if (!m_pListener)
        {
            log_error(helios_module, "CDH listener pointer is null!");
            return false;
        }
        m_pGtwConnector = new GatewayConnector(m_pListener);
    }

    bool ret = m_pGtwConnector->connect(ip.c_str(), port);
    if (ret)
        m_bConnected = true;
    else
        log_error(helios_module, "connect CDH failed!");

    return m_bConnected;
}

bool CDHClient::Login(const std::string &user, const std::string &password)
{
    bool ret = false;
    if ((!m_bConnected) || (nullptr == m_pGtwConnector))
    {
        log_error(helios_module, "connect first!");
        return ret;
    }

    if (nullptr == m_pToken)
    {
        ret = m_pGtwConnector->qbLogin(user.c_str(), password.c_str(), &m_pToken);
        if (ret && m_pToken)
            ret = true;
        else
            log_error(helios_module, "login failed!");
    }
    else
    {
        log_warn(helios_module, "already login.");
        ret = true;
    }

    return ret;
}

bool CDHClient::sendData(const int &funCode, const std::string &data)
{
    if (m_pToken && m_pGtwConnector)
    {
        bool ret = m_pGtwConnector->syncRequest(m_pToken, funCode, data.c_str());
        if (ret)
            return true;
        else
        {
            log_error(helios_module, "syncRequest failed!");
            return false;
        }
    }
    log_error(helios_module, "no connection or not login!");
    return false;
}

void CDHClient::getAuth()
{
    if (m_pToken && m_pGtwConnector)
        m_pGtwConnector->getAuthority(m_pToken);
}
