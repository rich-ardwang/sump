//-------------------------------------------------------------------------------------
//--Copyright	:	Sumscope
//--FileName	:	ISSMQObs.h
//--Author		:	cheng.xu@sumscope.com
//--Date		:	2015.01.23
//--Desc		:	��Ϣ������Ϣ�����ӿ�
//--History		:	Created by cheng.xu, 2015.01.23
//--				
//-------------------------------------------------------------------------------------

#ifndef __I_SS_MQOBS__
#define __I_SS_MQOBS__

#include "sdbus/message.h"

namespace SSMQ
{
	enum SSMQDeliverType
	{
		SSMQDT_Undefine,
		SSMQDT_Direct,
		SSMQDT_Publish
	};

	class ISSMQObs
	{
	protected:
		ISSMQObs(){}

	public:
		virtual ~ISSMQObs(){}

		//��������ע����Ϣtopic
		virtual sdbus::string GetBindKey() = 0;
		//��������ע����Ϣ���ͷ�ʽ
		virtual SSMQDeliverType GetDeliverType() = 0;
		//����������Ϣ
		virtual void OnMsgs(const sdbus::Message& msgs) = 0;
	};
}



#endif