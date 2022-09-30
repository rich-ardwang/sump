#include "stdafx.h"
#include "SSMQMsgSender.h"
#include "ISSMQMng.h"

bool CSSMQMsgSender::Send( const sdbus::string& bindKey, const sdbus::Message& request , sdbus::Message& response , int timeout /*= -1*/ )
{
    return GetSSMQMng()->Send(bindKey, request, response, timeout);
}

bool CSSMQMsgSender::Post( const sdbus::string& bindKey, const sdbus::Message& msg )
{
    return GetSSMQMng()->Post(bindKey, msg);
}

bool CSSMQMsgSender::Publish( const sdbus::string& bindKey, const sdbus::Message& msg )
{
    return GetSSMQMng()->Publish(bindKey, msg);
}
