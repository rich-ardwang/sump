
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
            ����ȴ��շ����
        */
        void close() ;
        /*
            ע�⣬���callback Ϊ�գ�ֻ��ͨ��recv��������Ϣ��
        */
        bool setHandler(const std::string& name, HANDLER_CALLBACK callback, void *clientdata , DeliverType type = DeliverDirect);
        bool setHandler(Callback* cb) ;
        bool setHandler(VectCallback* vcb) ;

        /*
            name��Ӧ�ڶ��е����֣�������message��subject��һ���֣�����Directģʽ��
            ÿ����Ϣֻ����һ��receiver�յ������DeliverTypeΪDeliverFanout�����Ӧ�ڶ��
            receiver����ͬ��fanout����
        */
        bool send(const std::string &name, const Message &message , DeliverType type = DeliverDirect);

        /*
            name��Ӧ��exchange�����֣�������message��subject��һ���֣�����fanoutģʽ��
            ÿ����Ϣ�����ж��receiver�յ���
        */
        bool fanout(const std::string& name , const Message &message) ;

        bool recv(Message& msg  ,  int timeout = 0); // seconds

        void log_level(int level) ;
        void log_tty_level(int level) ;

        /**
            1��SendCounter��ʾ���뷢�Ͷ�����Ϣ��
            2��SentCounter��ʾ�Ѿ����ͳɹ���Ϣ��
            3��RecvCounter��ʾ���뱾�ؽ��ն�����Ϣ��
            4��CallCounter��ʾ�Ѿ��ɹ�������Ϣ��

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
