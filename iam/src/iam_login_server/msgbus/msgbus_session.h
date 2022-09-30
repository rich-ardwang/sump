/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_session.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-17
*Desc        : manage the msgbus session.
*History     : Add by Lei.Wang 2019-09-17
*            : Mod by Lei.Wang 2019-10-23
***************************************************************************************************/
#ifndef __MSGBUS_SESSION_H__
#define __MSGBUS_SESSION_H__
#include "msgbus_base_session.h"

#define msgbus_session_moduler       "iam.login_server.msgbus_session"


class MsgbusSession : public Session {
public:
    MsgbusSession() {}
    ~MsgbusSession() {}

    void onMessage(const msgbus::Message &message);
    void onFanoutMessage(const msgbus::Message &message);

private:
    bool checkReplyQueue(const msgbus::Message &message);
    void procError(const msgbus::Message &message, int errCode);
    void procMsgParaseErr(const msgbus::Message &msg, bool isReply);
    void procServerStatusErr(const msgbus::Message &msg, bool isReply);
};

#endif /* __MSGBUS_SESSION_H__ */
