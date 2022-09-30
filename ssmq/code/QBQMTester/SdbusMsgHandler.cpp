#include "stdafx.h"
#include "SdbusMsgHandler.h"
#include <string>
#include "QMToQB\SSMQMsgSender.h"
#include "ISSMQMng.h"
#include "ssQBQMProto.h"
#include "SdbusMsgs.h"
using namespace std;

CMsgHandler::CMsgHandler()
{

}

CMsgHandler::~CMsgHandler()
{

}

sdbus::string CMsgHandler::GetBindKey()
{
    sdbus::string str("QMmsgQueue");
    return str;
}

SSMQ::SSMQDeliverType CMsgHandler::GetDeliverType()
{
    return SSMQ::SSMQDT_Direct;
}

void CMsgHandler::OnMsgs( const sdbus::Message& msgs )
{
    std::string sMsgID = msgs.GetMessageID();
    sdbus::string replyTo = msgs.GetReplyTo();
    const sdbus::Message::Map& values = msgs.MapValue();
    sdbus::string msgType = msgs.GetType();
    if (msgType == "ReqFromQB")
    {
        sdbus::Message msg;
        msg.SetMessageID(GetSSMQMng()->GetUniqueMsgID());
        msg.SetType("RetFromQM");
        msg.SetString(1, "RetFromQM");
        CSSMQMsgSender::Post(replyTo, msg);
    }
    else if (msgType == "sendFromQB")
    {
        sdbus::Message msg;
        msg.SetMessageID(sMsgID);
        msg.SetString(1, "hehe");
        CSSMQMsgSender::Post(replyTo, msg);
    }
    else if (msgType == "ComsRightQuery")
    {
        QB_QM_Com::SdbusMsgHeader head;
        QB_QM_Com::QM2QBComsRightQuery body;

        CSdbusMsgs msgParse;
        msgParse.ParseQM2QBComsRightQueryMsg(msgs, head, body);
        int i = 0;
        i--;
    }
}
