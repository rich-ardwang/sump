/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : CedaBusManager.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-07
  *Desc        : Ceda总线管理类，负责管理ceda/ATSMQ端的消息通道
  *History     : Add by Lei.Wang 2019-05-07
***************************************************************************************************/
#pragma once
#ifndef __CEDA_BUS_MANAGER_H__
#define __CEDA_BUS_MANAGER_H__
#include "CedaAgent.h"
#include "Config.h"


//#include <boost/serialization/singleton.hpp>
//#define GET_CEDA_BUS_MANAGER  CedaBusManager::get_mutable_instance()
//using boost::serialization::singleton;

#define GET_CEDA_BUS_MANAGER  GetCedaBusMngIns()



class CCedaAgent;
//class CedaBusManager : public singleton<CedaBusManager>
class CedaBusManager
{
public:
    CedaBusManager();
    virtual ~CedaBusManager();

    CCedaAgent *CreateCedaClient(const std::string &usr, const std::string &pwd, std::string &sLoginsid);
    CCedaAgent *CreateCedaClient(const std::string &loginMsg, std::string &sLoginsid);
    CCedaAgent *CreateCedaClient(const PxyInfo &pxyInfo, std::string &sLoginsid);
    bool removeCedaClient(CCedaAgent* pCedaAg);

private:
    void InitCedaProxy();

private:
    CedaProxy       *m_pCedaProxy;

};

CedaBusManager* GetCedaBusMngIns();

#endif //__CEDA_BUS_MANAGER_H__ endif