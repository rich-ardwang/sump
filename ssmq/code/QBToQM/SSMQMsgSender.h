#pragma once
#include "QB_QMDefine.h"
#include "sdbus/message.h"

class QB_QM_API CSSMQMsgSender
{
public:
    static bool Send(const sdbus::string& bindKey, const sdbus::Message& request, sdbus::Message& response, int timeout = -1);
    static bool Post(const sdbus::string& bindKey, const sdbus::Message& msg);
    static bool Publish(const sdbus::string& bindKey, const sdbus::Message& msg);
};
