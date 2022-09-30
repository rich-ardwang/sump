//-------------------------------------------------------------------------------------
//--Copyright	:	Sumscope
//--FileName	:	ISSMQDefine.h
//--Author		:	cheng.xu@sumscope.com
//--Date		:	2015.01.23
//--Desc		:	��Ϣ���߷�װʹ�õ��Ĺ�������
//--History		:	Created by cheng.xu, 2015.01.23
//                  Modified by lei.wang 2019-05-08
//--				
//-------------------------------------------------------------------------------------
#pragma once
#ifndef __I_SS_MQDEFINE__
#define __I_SS_MQDEFINE__

#include "sdbus\type.h"

#define SSMQ_ENGINE	"zmq"
#define SSMQ_SRVNAME _T("SSMQServer.exe")
#define SSMQ_TOPIC_MNG "ssmq_topic_mng"
#define SSMQ_CMD_ADDCLIENT "ssmq_cmd_addclient"
#define SSMQ_CMD_REMOVECLIENT "ssmq_cmd_removeclient"
#define SSMQ_CMD_ACKSRVPROCID "ssmq_cmd_acksrvprocid"
#define SSMQ_CMD_CLOSEALL "ssmq_cmd_closeall"
#define SSMQ_CMD_CLOSECLIENT "ssmq_cmd_closeclient"

/* Ceda ATS MQ */
#define CEDA_ATS_MQ_HEADER              "ceda"
#define CEDA_ATS_MQ_MANAGE              "mng"
#define CEDA_ATS_MQ_EVENT               "event"
#define CEDA_ATS_MQ_TOPIC               "topic"
#define CEDA_ATS_MQ_SEND                "send"
#define CEDA_CMD_CEDA_INIT              "ceda_cmd_cedaInit"
#define CEDA_CMD_CEDA_INIT_ACK          "ceda_cmd_cedaInit_ack"
#define CEDA_CMD_CLOSE_CLIENT           "ceda_cmd_close_client"
#define CEDA_CMD_LOGIN_WITH_USER        "ceda_cmd_login_with_user"
#define CEDA_CMD_LOGIN_WITH_JSON        "ceda_cmd_login_with_json"
#define CEDA_CMD_LOGIN_ACK              "ceda_cmd_login_ack"
#define CEDA_CMD_UNREG_TO_SERVER        "ceda_cmd_unreg_to_server"
#define CEDA_CMD_CEDA_SUBSCRIBE         "ceda_cmd_ceda_subscribe"
#define CEDA_CMD_CEDA_SUBSCRIBE_ACK     "ceda_cmd_ceda_subscribe_ack"
#define CEDA_CMD_CEDA_UNSUBSCRIBE       "ceda_cmd_ceda_unsubscribe"
#define CEDA_CMD_CEDA_UNSUBSCRIBE_ACK   "ceda_cmd_ceda_unsubscribe_ack"
#define CEDA_CMD_CEDA_SEND              "ceda_cmd_ceda_send"
#define CEDA_CMD_CEDA_SEND_ACK          "ceda_cmd_ceda_send_ack"
#define CEDA_CMD_CEDA_POST              "ceda_cmd_ceda_post"
#define CEDA_LOGIN_RECV_TOPIC           "SYS.ATS.LOGIN"
#define CEDA_DISPATCH_RES_MSG           "ceda_dispatch_res_msg"


const sdbus::uint32_t SSMQ_MSGIDX_CMDTYPE = 1;
const sdbus::uint32_t SSMQ_MSGIDX_CMDPARA = 2;

/*Ceda ATS MQ Data Def */
enum CedaServerEventType {
    CONNECTION_CONNECTING = 0,              //��������
    CONNECTION_CONNECTED = 1,               //������
    CONNECTION_CLOSED = 2,                  //���ӹر�
    CONNECTION_IO_EXCEPTION = 3,            //����IO����
    CONNECTION_RECONNECT = 4,               //��������
    CONNECTION_LOGINING = 6,                //���ڵ�¼
    CONNECTION_LOGIN_SUCCESS = 7,           //��¼�ɹ�
    CONNECTION_LOST = 8,                    //���Ӷ�ʧ
    CONNECTION_TIMEOUT = 9,                 //���ӳ�ʱ
    CONNECTION_EXCEPTION = 10,

    ERR_CONNECTION_UNKNOW = 40,             //����δ֪����
    ERR_CONNECTION_NULL_USER_PASSWD = 41,   //�������
    ERR_CONNECTION_USER_PASSWD = 42,        //�������
    ERR_CONNECTION_CLIENTID_EXIST = 43,     //ClientID exist
    ERR_CONNECTION_AUTHORIZE_CA = 44,       //δ��Ȩ CA
    ERR_CONNECTION_CHECK_CA = 45,           //CA
    ERR_CONNECTION_READ_CA = 46,            //CA

    ERR_SUBSCRIB_UNKNOW=60,                 //����δ֪����
    SUCCESS_SUBSCRIB=61,                    //���ĳɹ�
    ERR_SUBSCRIB_PERIMISSION=62,            //NO ����Ȩ��
    ERR_SUBSCRIB_LOGIN=63,                  //����ʱδ��¼

    ERR_PUBLISH_UNKNOW=70,                  //����δ֪����
    ERR_PUBLISH_LOGIN=71,                   //NO Login
    ERR_PUBLISH_PERIMISSION=72              //NO ����Ȩ��
};

enum CedaClientEventType {
    CEDA_LOGIN_OK = 0,                       //ceda login ok
    CEDA_LOGIN_FAILED = 1,                   //ceda login failed
    CEDA_LOGIN_INVALID_KEY = 2,              //invalid pass key
    CEDA_SUBSCRIBE_OK = 3,                   //ceda subscribe ok
    CEDA_SUBSCRIBE_TOPIC_EMPTY = 4,          //ceda subscribe topic is empty
    CEDA_ALREADY_SUBSCRIBE = 5               //ceda allready subscribe
};

#endif