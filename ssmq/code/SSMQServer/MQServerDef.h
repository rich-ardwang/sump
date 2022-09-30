/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : MQServerDef.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-16
  *Desc        : SSMQServer端使用的一些数据类型定义在这个文件中
  *History     : Add by Lei.Wang 2019-05-16
***************************************************************************************************/
#pragma once
#ifndef __MQ_SERVER_DEF_H__
#define __MQ_SERVER_DEF_H__
#include <string>
#include <set>
#include "busmap.h"
#include "BlockingQueue.h"


/* 管理通道和事件通道 */
typedef struct stCedaCliMngObsBindKey {
    std::string sCedaMngKey;
    std::string sCedaEventKey;
} MngObs;

/* Topic监听通道 */
typedef struct stCedaCliTopicObsBindKey {
    std::string sTopicObsKey;
    std::string sTopics;
    std::string sSrvID;
} TopicObs;

/*
    为每个Ceda客户端进程维护一个管理通道和一个事件通道
    Key:process id，Value:MngObs
*/
typedef busmap::map<std::string, MngObs> CedaCliObsMap;

/*
    为每个Ceda客户端维护订阅主题，服务端收到订阅消息后据此派发给各个客户端
    Key:topic，Value:子busmap
    子busmap，Key:TopicObsKey，Value:TopicObs
*/
typedef busmap::map<std::string, std::map<std::string, TopicObs> > TopicsDestinationMap;

/*
    为每个Ceda客户端进程维护若干个Topic监听通道，利于客户端并发处理业务
    Key:TopicObsKey，Value:此通道监听的所有topics(去重)
*/
typedef busmap::map<std::string, std::set<std::string> > TopicObsMap;

/* 线程矢量 */
typedef std::vector<boost::thread *> ThreadVector;

/*
    Ceda客户端管理操作请求
    管理操作主要是Ceda初始化、Login、客户端连接关闭等
*/
typedef struct stCedaMngReq {
    std::string     sCedaCmd;
    std::string     sUsername;
    std::string     sPwd;
    std::string     sValidkey;
    MngObs          stMngObs;
} CedaMngReq;

/*
    Ceda客户端订阅操作请求
    客户端发起的批量订阅与退订业务
*/
typedef struct stCedaSubscribeReq {
    std::string     sCedaCmd;
    TopicObs        stTopicObs;
} CedaSubscReq;

/*
    从Ceda后台服务回来的消息
    由dispatch工作线程组统一派发给相应通道
    这些消息是订阅消息与异步接口回调消息
*/
typedef struct stCedaDispatchMsg {
    std::string     sTopic;
    std::string     sContent;
    int             nLen;
} CedaDisMsg;

/*
    Ceda客户端同步发送消息操作请求
*/
typedef struct stCedaSendReq {
    std::string     sSendBindkey;
    std::string     sSendTopic;
} CedaSendReq;

/* Ceda客户端管理操作请求队列 */
typedef BlockingQueue<CedaMngReq *> CedaManageReq;

/* Ceda客户端订阅操作请求队列 */
typedef BlockingQueue<CedaSubscReq *> CedaSubReq;

/* Ceda服务消息分发队列 */
typedef BlockingQueue<CedaDisMsg *> CedaDispatchMsg;

/* Ceda客户端同步接口消息分发队列 */
typedef BlockingQueue<CedaSendReq *> CedaSendMsg;


#endif  //__MQ_SERVER_DEF_H__