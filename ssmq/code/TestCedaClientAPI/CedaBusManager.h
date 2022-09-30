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
#include <iostream>
#include <boost/serialization/singleton.hpp>

#define GET_CEDA_BUS_MANAGER  CedaBusManager::get_mutable_instance()

using boost::serialization::singleton;
using namespace std;


class CedaBusManager : public singleton<CedaBusManager>
{
public:
    CedaBusManager();
    virtual ~CedaBusManager();

    CCedaAgent *CreateCedaClient(const std::string &usr, const std::string &pwd);
    CCedaAgent *CreateCedaClient(const std::string &loginMsg);
    CCedaAgent *CreateCedaClient(const PxyInfo &pxyInfo);
    bool removeCedaClient(CCedaAgent* pCedaAg);


private:
    void InitCedaProxy();

private:
    CedaProxy       *m_pCedaProxy;

};


#endif //__CEDA_BUS_MANAGER_H__ endif