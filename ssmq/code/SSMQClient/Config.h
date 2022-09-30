/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : Config.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-07
  *Desc        : SSMQClient全局配置
  *History     : Add by Lei.Wang 2019-05-07
***************************************************************************************************/
#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
#include "SSRTL\SSLog.h"
#include "LogMarco.h"


#define GET_GOL_CONF    CConfig::getInstance()->GetGlobalConf()

using namespace std;

/* 全局配置 */
typedef struct tagConf {
    string      sBrokerUrl;
    string      sCedaBindKeySpr;
} gConf;

class CConfig
{
public:
    static CConfig *getInstance() {
        static CConfig _inst;
        return &_inst;
    }

public:
    gConf GetGlobalConf();

private:
    CConfig();
    CConfig(string path);
    CConfig(const CConfig &);
    CConfig &operator=(const CConfig &);
    t_string GetModuleDirPath();
    void loadGlobalConfig();

private:
    string      m_confPath; //配置文件的路径
    gConf       m_gConf;    //全局配置
};

ss::logger* GetCedaCliLogger();

#endif //__CONFIG_H__ endif