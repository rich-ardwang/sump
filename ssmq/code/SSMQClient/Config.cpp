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
    GetPrivateProfileString(_T("maincfg"), _T("cedakeySepr"), _T("."), buff, MAX_PATH, m_confPath.c_str());
    m_gConf.sCedaBindKeySpr = SS_BASE::t_string2string(buff);
}

//CedaMQClient log
ss::logger* GetCedaCliLogger() {
    static ss::logger sRet(".\\log\\SSMQ_CEDAMQ_Client");
    sRet.set_level_by_config(".\\config\\logconfig.ini", "maincfg.ssmqclient");
    return &sRet;
}
