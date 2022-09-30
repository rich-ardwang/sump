
#ifndef __MSGBUS_CONNECTION_H
#define __MSGBUS_CONNECTION_H 1

#include "msgbus/message.h"
#include "msgbus/callback.h"

namespace msgbus {

    class ConnectionImpl;
    class MSGBUSAPI Connection {
    public:
        Connection();
        ~Connection();

        bool connect(const std::string &url, bool reconnect = true);

        /*
            必须等待收发完成
        */
        void close() ;
        /*
            注意，如果callback 为空，只能通过recv来接收消息。
        */
        bool setHandler(const std::string& name, HANDLER_CALLBACK callback, void *clientdata , DeliverType type = DeliverDirect);
        bool setHandler(Callback* cb) ;
        bool setHandler(VectCallback* vcb) ;

        /*
            name对应于队列的名字，必须是message中subject的一部分，这是Direct模式，
            每个消息只会有一个receiver收到。如果DeliverType为DeliverFanout，则对应于多个
            receiver，等同于fanout函数
        */
        bool send(const std::string &name, const Message &message , DeliverType type = DeliverDirect);

        /*
            name对应于exchange的名字，必须是message中subject的一部分，这是fanout模式，
            每个消息可以有多个receiver收到。
        */
        bool fanout(const std::string& name , const Message &message) ;

        bool recv(Message& msg  ,  int timeout = 0); // seconds

        void log_level(int level) ;
        void log_tty_level(int level) ;

        /**
            1、SendCounter表示进入发送队列消息数
            2、SentCounter表示已经发送成功消息数
            3、RecvCounter表示进入本地接收队列消息数
            4、CallCounter表示已经成功调用消息数

        */
        size_t SendCounter() const ;
        size_t SentCounter() const ;
        size_t RecvCounter() const ;
        size_t CallCounter() const ;

    protected:
        ConnectionImpl *impl_;
    };

}

#endif  /** __MSGBUS_CONNECTION_H */
