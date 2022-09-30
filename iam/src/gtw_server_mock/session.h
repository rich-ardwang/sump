#pragma once
#include "msgbus_base_session.h"

#define session_moduler       "iam.test.gtw_server_mock.session"

using msgbus::Message;
using msgbus::Connection;

class GtwServerSession : public Session {
public:
    GtwServerSession() {}
    ~GtwServerSession() {}

    virtual void onMessage(const Message &message);
    virtual void onFanoutMessage(const Message &message) {}

private:
};
