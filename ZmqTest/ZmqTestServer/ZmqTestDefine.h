/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ZmqTestDefine.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/27
  *Desc        :  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others    :  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History    :  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#ifndef ZMQTESTDEFINE_H_10BFAFD1_971B_4DE0_AC21_079ABE4142F2
#define ZMQTESTDEFINE_H_10BFAFD1_971B_4DE0_AC21_079ABE4142F2

#include "sdbus\type.h"

#define  ZMQ_ENGINE  "testzmq"
#define  ZMQ_SEVNAME "ZmqTestServer.exe"
#define  ZMQ_TOPIC_MNG "ssmq_topic_mng"
#define  ZMQ_TOPIC_MNG_CLIENT "ssmq_topic_client_mng"
#define  ZMQ_CMD_ADDCLIENT "ssmq_cmd_addclient"
#define  ZMQ_CMD_REMOVECLIENT "ssmq_cmd_removeclient"
#define  ZMQ_CMD_ACKSRVPROCID "ssmq_cmd_acksrvprocid"
#define  ZMQ_CMD_CLOSEALL "ssmq_cmd_closeall"
#define  ZMQ_CMD_CLOSECLIENT "ssmq_cmd_closeclient"

const sdbus::uint32_t ZMQ_MSGIDX_CMDTYPE = 1;
const sdbus::uint32_t ZMQ_MSGIDX_CMDPARA = 2;

#endif