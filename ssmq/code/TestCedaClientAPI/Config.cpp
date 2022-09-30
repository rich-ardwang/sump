#include "stdafx.h"
#include "Config.h"



gConf CConfig::GetGlobalConf()
{
    return m_gConf;
}

CConfig::CConfig()
{
    m_confPath = GetModuleDirPath();
    m_confPath += _T("config\\ssmg.ini");
    loadGlobalConfig();
}

CConfig::CConfig(string path) : m_confPath(path)
{
    loadGlobalConfig();
}

t_string CConfig::GetModuleDirPath()
{
    TCHAR szFileNames[260];
    DWORD dwLen = GetModuleFileName(NULL, szFileNames, sizeof(szFileNames));
    for(DWORD offset=dwLen; offset>=0; offset--) {
        if(szFileNames[offset] == _T('\\')) {
            szFileNames[offset] = _T('\0');
            break;
        }
    }
    t_string sRet = szFileNames;
    sRet += _T("\\");
    return sRet;
}

void CConfig::loadGlobalConfig()
{
    TCHAR buff[MAX_PATH] = {_T('\0')};
    GetPrivateProfileString(_T("maincfg"), _T("brokerurl"), _T("tcp://127.0.0.1:7788"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.sBrokerUrl = SS_BASE::t_string2string(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("msgqueuesize"), _T("128"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.nMsgQueueSize = _ttoi(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("gatewayHost"), _T("172.16.97.106"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stGtwInfo.sGtwHost = SS_BASE::t_string2string(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("gatewayPort"), _T("9060"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stGtwInfo.nGtwPort = _ttoi(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("gatewayProtocol"), _T("1"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stGtwInfo.nGtwProtocol = _ttoi(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("gatewayIsGzip"), _T("1"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stGtwInfo.nGtwIsGZip = _ttoi(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("proxyEnable"), _T("0"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stPxyInfo.nPxyEnable = _ttoi(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("proxyType"), _T("1"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stPxyInfo.nPxyType = _ttoi(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("proxyHost"), _T("127.0.0.1"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stPxyInfo.sPxyHost = SS_BASE::t_string2string(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("proxyPort"), _T("8899"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stPxyInfo.nPxyPort = _ttoi(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("proxyUser"), _T("sumscope"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stPxyInfo.sPxyUsr = SS_BASE::t_string2string(buff);

    buff[0] = '\0';
    GetPrivateProfileString(_T("maincfg"), _T("proxyPwd"), _T("654321"), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.stPxyInfo.sPxyPwd = SS_BASE::t_string2string(buff);
}

//SSMQServer log
ss::logger* GetSrvLogger() {
    static ss::logger sRet("log\\SSMQServer");
    sRet.set_level_by_config("config\\logconfig.ini", "maincfg.ssmqserver");
    return &sRet;
}

