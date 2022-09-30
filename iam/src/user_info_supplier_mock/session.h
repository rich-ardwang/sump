#pragma once
#include "msgbus_base_session.h"

#define session_moduler       "iam.test.user_info_supplier.session"

using msgbus::Message;
using msgbus::Connection;

class UserinfoSession : public Session {
public:
    UserinfoSession() {}
    ~UserinfoSession() {}

    virtual void onMessage(const Message &message);
    virtual void onFanoutMessage(const Message &message);

private:
};
