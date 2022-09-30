#ifndef SSQBQM_PROTO_H_
#define SSQBQM_PROTO_H_

#include <string>
#include <list>

//代码中用宏来判断消息类型
#define  SSCMD_ComsRightQuery 							"ComsRightQuery"								//QM从QB查询资金权限	
#define  SSCMD_ComsWhiteListQuery 						"ComsWhiteListQuery"							//QM从QB客户端查询本人白名单
#define  SSCMD_AllUnreadMsgQuery 						"AllUnreadMsgQuery"								//QB从QM查询所有未读消息数目的接口
#define  SSCMD_UnreadMsgQuery 							"UnreadMsgQuery"								//QB从QM查询某人未读消息数目的接口
#define  SSCMD_UnreadMsgRegister 						"UnreadMsgRegister"								//QM某人未读消息数目变化的时候主动通知QB的注册
#define  SSCMD_UnreadMsgChanged_Push 					"UnreadMsgChanged_Push"							//QM某人未读消息数目变化的时候主动通知QB
#define  SSCMD_OpenQMMainWindow 						"OpenQMMainWindow"								//QB调起QM主窗口
#define  SSCMD_OpenQMDialog 					 		"OpenQMDialog"									//QB调起与某人QM对话的接口
#define  SSCMD_OpenComsPage 					 		"OpenComsPage"									//QM通过QB调起COMS页面的接口



namespace QB_QM_Com
{

    typedef struct stSdbusMsgHeader
    {
        stSdbusMsgHeader()
            : errorCode(0)
            , totalNum(0)
            , index(0)
        {

        }
        stSdbusMsgHeader(int eCode, const std::string &eMsg, int count, int idx)
            : errorCode(eCode)
            , errorMsg(eMsg)
            , totalNum(count)
            , index(idx)
        {
        }
        int errorCode;//错误代码 0=正确,其他值业务自定
        std::string errorMsg;//错误描述
        int totalNum;//总包数
        int index;//当前包数
    }SdbusMsgHeader;

	//------------------------------------
	//10.1 QM从QB查询资金权限	
	//MsgType:ComsRightQuery
	//备注
	//Req
	typedef struct stQM2QBComsRightQuery
	{
		std::string sUserID;	//用户32位ID
	}QM2QBComsRightQuery;

	//ack
	typedef struct stQM2QBComsRightQueryAck
	{
		std::string sUserID;	//用户32位ID
		std::string sValue; 	//权限值
	}QM2QBComsRightQueryAck;
	
	
	//------------------------------------
	//QM从QB客户端查询本人白名单
	//MsgType:ComsWhiteListQuery
	//备注
	//Req
	typedef struct stQM2QBComsWhiteListQuery 
	{
		std::string sUserID; //用户32位ID
	}QM2QBComsWhiteListQuery;

	//ack
	typedef struct stComsAdmissionUnit
	{
		std::string sCompanyID;	//机构32位ID
		std::string sValue;		//权限值，授权结果值,按业务定义,暂定: 允许-1, 不允许-0，2--黑名单
		
	}ComsAdmissionUnit;
	
	typedef struct stQM2QBComsWhiteListQueryAck
	{
		std::string sUserID;	//用户32位ID
		std::list<ComsAdmissionUnit> lstWhite;
		
	}QM2QBComsWhiteListQueryAck;
	
	
	//------------------------------------
	//QB从QM查询所有未读消息数目的接口
	//MsgType:AllUnreadMsgQuery 
	//备注: 暂不实现
	 
	//------------------------------------
	//QB从QM查询某人未读消息数目的接口
	//MsgType:UnreadMsgQuery  
	//备注: 
	//Req
	typedef struct stQB2QMUnreadMsgQuery
	{
		std::list<std::string> lstUserID;	//用户32位ID
	}QB2QMUnreadMsgQuery;

	//ack
	typedef struct stUnreadMsgUnit
	{
		std::string sUserID;	//用户32位ID
		int nValue;				//未读消息数
		
	}UnreadMsgUnit;	
	
	typedef struct stQB2QMUnreadMsgQueryAck
	{
		std::list<UnreadMsgUnit> lstUnreadMsg;	//未读消息列表
	}QB2QMUnreadMsgQueryAck;	
	
	
	//------------------------------------
	//QM某人未读消息数目变化的时候主动通知QB的注册
	//MsgType:UnreadMsgRegister 
	//备注: 
	typedef struct stQB2QMUnreadMsgRegister
	{
		std::list<std::string> lstUserID;	//用户32位ID
		bool bRegisterFlag;								//true 注册，false 取消注册
	}QB2QMUnreadMsgRegister;

	//ack
	//无应答包体
	
	
	//------------------------------------
	//QM某人未读消息数目变化的时候主动通知QB
	//MsgType:UnreadMsgChanged.Push
	//备注: 
	//推送结构 UnreadMsgUnit


	//------------------------------------
	//QB调起QM主窗口
	//MsgType:OpenQMMainWindow
	//备注: 
	//Req 无包体
	//ack 无包体
	
	
	//------------------------------------
	//QB调起与某人QM对话的接口
	//MsgType:OpenQMDialog 
	//备注: 
	//Req 
	typedef struct stQB2QMOpenQMDialog
	{
		std::string sUserID;		//用户32位ID
		std::string sUserName;	//用户帐号
			
	}QB2QMOpenQMDialog;
	//ack 无包体
	
	
	//------------------------------------
	//QM通过QB调起COMS页面的接口
	//MsgType:OpenComsPage 
	//备注: 
	//Req 
	typedef struct stOpenComsPage
	{
		std::string sUserID;		//用户32位ID
		std::string sUserName;	//用户帐号
			
	}OpenComsPage;
	//ack 无应答
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif

