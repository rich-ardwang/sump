#ifndef SSQBQM_PROTO_H_
#define SSQBQM_PROTO_H_

#include <string>
#include <list>

//�������ú����ж���Ϣ����
#define  SSCMD_ComsRightQuery 							"ComsRightQuery"								//QM��QB��ѯ�ʽ�Ȩ��	
#define  SSCMD_ComsWhiteListQuery 						"ComsWhiteListQuery"							//QM��QB�ͻ��˲�ѯ���˰�����
#define  SSCMD_AllUnreadMsgQuery 						"AllUnreadMsgQuery"								//QB��QM��ѯ����δ����Ϣ��Ŀ�Ľӿ�
#define  SSCMD_UnreadMsgQuery 							"UnreadMsgQuery"								//QB��QM��ѯĳ��δ����Ϣ��Ŀ�Ľӿ�
#define  SSCMD_UnreadMsgRegister 						"UnreadMsgRegister"								//QMĳ��δ����Ϣ��Ŀ�仯��ʱ������֪ͨQB��ע��
#define  SSCMD_UnreadMsgChanged_Push 					"UnreadMsgChanged_Push"							//QMĳ��δ����Ϣ��Ŀ�仯��ʱ������֪ͨQB
#define  SSCMD_OpenQMMainWindow 						"OpenQMMainWindow"								//QB����QM������
#define  SSCMD_OpenQMDialog 					 		"OpenQMDialog"									//QB������ĳ��QM�Ի��Ľӿ�
#define  SSCMD_OpenComsPage 					 		"OpenComsPage"									//QMͨ��QB����COMSҳ��Ľӿ�



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
        int errorCode;//������� 0=��ȷ,����ֵҵ���Զ�
        std::string errorMsg;//��������
        int totalNum;//�ܰ���
        int index;//��ǰ����
    }SdbusMsgHeader;

	//------------------------------------
	//10.1 QM��QB��ѯ�ʽ�Ȩ��	
	//MsgType:ComsRightQuery
	//��ע
	//Req
	typedef struct stQM2QBComsRightQuery
	{
		std::string sUserID;	//�û�32λID
	}QM2QBComsRightQuery;

	//ack
	typedef struct stQM2QBComsRightQueryAck
	{
		std::string sUserID;	//�û�32λID
		std::string sValue; 	//Ȩ��ֵ
	}QM2QBComsRightQueryAck;
	
	
	//------------------------------------
	//QM��QB�ͻ��˲�ѯ���˰�����
	//MsgType:ComsWhiteListQuery
	//��ע
	//Req
	typedef struct stQM2QBComsWhiteListQuery 
	{
		std::string sUserID; //�û�32λID
	}QM2QBComsWhiteListQuery;

	//ack
	typedef struct stComsAdmissionUnit
	{
		std::string sCompanyID;	//����32λID
		std::string sValue;		//Ȩ��ֵ����Ȩ���ֵ,��ҵ����,�ݶ�: ����-1, ������-0��2--������
		
	}ComsAdmissionUnit;
	
	typedef struct stQM2QBComsWhiteListQueryAck
	{
		std::string sUserID;	//�û�32λID
		std::list<ComsAdmissionUnit> lstWhite;
		
	}QM2QBComsWhiteListQueryAck;
	
	
	//------------------------------------
	//QB��QM��ѯ����δ����Ϣ��Ŀ�Ľӿ�
	//MsgType:AllUnreadMsgQuery 
	//��ע: �ݲ�ʵ��
	 
	//------------------------------------
	//QB��QM��ѯĳ��δ����Ϣ��Ŀ�Ľӿ�
	//MsgType:UnreadMsgQuery  
	//��ע: 
	//Req
	typedef struct stQB2QMUnreadMsgQuery
	{
		std::list<std::string> lstUserID;	//�û�32λID
	}QB2QMUnreadMsgQuery;

	//ack
	typedef struct stUnreadMsgUnit
	{
		std::string sUserID;	//�û�32λID
		int nValue;				//δ����Ϣ��
		
	}UnreadMsgUnit;	
	
	typedef struct stQB2QMUnreadMsgQueryAck
	{
		std::list<UnreadMsgUnit> lstUnreadMsg;	//δ����Ϣ�б�
	}QB2QMUnreadMsgQueryAck;	
	
	
	//------------------------------------
	//QMĳ��δ����Ϣ��Ŀ�仯��ʱ������֪ͨQB��ע��
	//MsgType:UnreadMsgRegister 
	//��ע: 
	typedef struct stQB2QMUnreadMsgRegister
	{
		std::list<std::string> lstUserID;	//�û�32λID
		bool bRegisterFlag;								//true ע�ᣬfalse ȡ��ע��
	}QB2QMUnreadMsgRegister;

	//ack
	//��Ӧ�����
	
	
	//------------------------------------
	//QMĳ��δ����Ϣ��Ŀ�仯��ʱ������֪ͨQB
	//MsgType:UnreadMsgChanged.Push
	//��ע: 
	//���ͽṹ UnreadMsgUnit


	//------------------------------------
	//QB����QM������
	//MsgType:OpenQMMainWindow
	//��ע: 
	//Req �ް���
	//ack �ް���
	
	
	//------------------------------------
	//QB������ĳ��QM�Ի��Ľӿ�
	//MsgType:OpenQMDialog 
	//��ע: 
	//Req 
	typedef struct stQB2QMOpenQMDialog
	{
		std::string sUserID;		//�û�32λID
		std::string sUserName;	//�û��ʺ�
			
	}QB2QMOpenQMDialog;
	//ack �ް���
	
	
	//------------------------------------
	//QMͨ��QB����COMSҳ��Ľӿ�
	//MsgType:OpenComsPage 
	//��ע: 
	//Req 
	typedef struct stOpenComsPage
	{
		std::string sUserID;		//�û�32λID
		std::string sUserName;	//�û��ʺ�
			
	}OpenComsPage;
	//ack ��Ӧ��
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////


#endif

