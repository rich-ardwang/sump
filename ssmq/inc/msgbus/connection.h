
#ifndef __MSGBUS_CONNECTION_H
#define __MSGBUS_CONNECTION_H 1

#include "msgbus/compile.h"
#include "msgbus/type.h"
#include "sdbus/connection.h"
#include "msgbus/message.h"
#include "msgbus/callback.h"

namespace msgbus{

class MSGBUSAPI Connection : public sdbus::Connection{
public:
    Connection() ;
    ~Connection() ;

    /*
        ��Ҫע����ǣ�����汾Ϊ�˼���databus��url��������Ѿ�ʧЧ�ˡ���clientcfg.xml�����á�
    */
    bool connect(const msgbus::string& url) ;
    void close() ;

    /*
        ���������ĳ��topic�Ļص���������ô���Զ����ĸ÷����£����е����⡣����DataDictionary.xml
    */
    bool setHandler(const msgbus::string& name, HANDLER_CALLBACK callback, void *clientdata , DeliverType type = DeliverDirect);
    bool setHandler(Callback* cb) ;
    bool setHandler(VectCallback* vcb) ;

    bool send(const msgbus::string &name, const Message &message , DeliverType type = DeliverDirect);

    bool fanout(const msgbus::string& name , const Message &message) ;

    bool recv(Message& msg  ,  int timeout = 0);    

    void log_level(int level) {sdbus::Connection::LogLevel(level) ;}
    inline void log_tty_level(int level) {}

private:
    bool add_handler(VectCallback* vcb) ;
} ;

}
#endif  /** __MSGBUS_CONNECTION_H */
