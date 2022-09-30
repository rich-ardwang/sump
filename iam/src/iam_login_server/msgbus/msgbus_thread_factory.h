/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : msgbus_thread_factory.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-09-17
*Desc        : msgbus thread factory.
*History     : Add by Lei.Wang 2019-09-17
***************************************************************************************************/
#ifndef __MSGBUS_THREAD_FACTORY_H__
#define __MSGBUS_THREAD_FACTORY_H__
#include <boost/shared_ptr.hpp>
#include "msgbus_thread.h"
#include "msgbus_type.h"

#define msgbus_thread_factory_moduler       "iam.login_server.msgbus_thread_factory"


class MsgbusThreadFactory {
public:
    static boost::shared_ptr<MsgbusThread> concrete(emMsgbusType type, 
        const std::string &url, const std::string &que_name, 
        boost::shared_ptr<Session> session);
};

#endif /* __MSGBUS_THREAD_FACTORY_H__ */

