
#ifndef __SDBUS_CONNECTION_H
#define __SDBUS_CONNECTION_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/callback.h"
#include "sdbus/engine.h"

namespace sdbus{

static const int DeliverDirect  = 0 ;       //�ȼ��� Request   / Response
static const int DeliverPublish = 1 ;       //�ȼ��� Subscribe / Publish

class SDBUSAPI Connection{
public:
    Connection() ;
    ~Connection() ;

    /*
        ��url�ĺ�������engineָ���Ĵ��������������ģ�Ŀǰ֧��qpid���档
    */
    bool Connect(const sdbus::string& url , const sdbus::string& engine = "qpid") ;
    void Close() ;

    bool AddHandler(Callback * cb) ;
    bool AddHandler(const sdbus::string& name , int type , Callback * cb) ;
    void DelHandler(const Callback * cb) ;

    bool Send(const sdbus::string& name , const Message& msg) ;
    
    /*
        2014-11-05
        �������Ͽ����Ҳ�����֧��ͬ��ģʽ�����Ǵ�ҵ��Ƕ�������ͬ��ģʽ�������õĳ�����
        ��������ͨ����Ӧ��ͨ������ͬһ��ͨ��������ڷ���ͬ������֮ǰ�����뱣֤�Ѿ�ע����
        һ��Callback������Ҫ��֤���Callback�Ķ�����û�б����������ã�����ȫ��Ψһ�ԡ�
        ��request��ReplyTo�����������Callback����������ô����˾Ϳ���ͨ��������з���
        Ӧ��Ľ����
        request��MessagID�ڵײ������У��ᱻ�滻Ϊ����Ψһ��ID����response����ʱ��Ӧ�ò��
        MessageID�����
        �����ʱ���󣬼�ʹӦ�𷵻أ����Ӧ����ϢҲ�ᱻ���������Ӧ����Ϣ�����������ôҲֻ��
        ������һ����������������
    */
    bool Send(const sdbus::string& name , const Message& request , Message& response , int timeout = -1) ;

    bool Publish(const sdbus::string& topic , const Message& msg) ;

    bool SetOptions(const Options& options) ;
    const Options& GetOptions() const;

    static void LogLevel(int level) ;
    static int LogLevel() ;
    
private:

    Engine * engine_ ;
} ;

}

#endif  /** __SDBUS_CONNECTION_H */
