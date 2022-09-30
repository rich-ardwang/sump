/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_base_session.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-11-04
*Desc        : base session of message bus, user will inherit this class and 
*            : implement it as an observer.
*History     : Created by Lei.Wang 2019-11-04
***************************************************************************************************/
#ifndef __MSGBUS_BASE_SESSION_H__
#define __MSGBUS_BASE_SESSION_H__
#include "msgbus/connection.h"
#include "msgbus/message.h"


class Session {
public:
    Session() {}
    virtual ~Session() {}

    inline void set_connection(msgbus::Connection *con) {
        con_ = con;
    }

    virtual void onMessage(const msgbus::Message &message) {}
    virtual void onFanoutMessage(const msgbus::Message &message) {}

protected:
    msgbus::Connection      *con_;
};

#endif /* __MSGBUS_BASE_SESSION_H__ */
