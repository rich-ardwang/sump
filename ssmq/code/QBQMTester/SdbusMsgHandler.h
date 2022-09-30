#pragma once
#include "ISSMQObs.h"

class CMsgHandler
    : public SSMQ::ISSMQObs
{
public:
    CMsgHandler();
    ~CMsgHandler();
    virtual sdbus::string GetBindKey();
    virtual SSMQ::SSMQDeliverType GetDeliverType();
    virtual void OnMsgs(const sdbus::Message& msgs);


    
};
