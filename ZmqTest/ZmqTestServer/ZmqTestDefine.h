/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName    :  ZmqTestDefine.h
  *Author    :  shangyi.xin
  *Version    :  1.0
  *Date        :  2019/03/27
  *Desc        :  //������Ҫ˵���˳����ļ���ɵ���Ҫ����
  *Relation :  
  *Others    :  //��������˵��
  *Function :  //��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
  *History    :  //�޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸��߼��޸����ݼ��
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