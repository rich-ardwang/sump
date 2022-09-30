
#ifndef __SDBUS_CALLBACK_H
#define __SDBUS_CALLBACK_H 1

#include "sdbus/message.h"
#include "sdbus/dict.h" 
#include "sdbus/options.h"

namespace sdbus{

class Connection ;
class CallbackImpl ;
class SDBUSAPI Callback{
public:
    Callback() ;
    virtual ~Callback() ;

    /*
        2015-01-09
        Ϊ�˽����ʱ���ӵ�qpid������ǻ��������ӣ�����ΪTCP�����ӵ��ٶȱȽ���������qpid
        ��Ӧ�����Ѿ�ʧЧ�����ӡ�

        ��Ҫע����ǣ�
        1����type == DeliverPublish����ôdurable������Ч������Ϊfalse���ǳ־û��ġ�
        2����type == DeliverDirectʱ��durableĬ��Ϊtrue����ʱname����ǿա�
        3����type == DeliverDirect��ͬʱdurable == false ����ô name��Ч��
           ��ʱ��ϵͳ�Զ�Ϊ������UUID��Ϊ��������

        ��Ҫע����ǣ������������Ϊϵͳ�Զ�����ʱ��callback�����ƻ��ڶϿ�����ʱ��
        �����仯����ˣ���ÿ�η���Ϣʱ����������ȡ�ø�callback��name����ΪReplyTo��
    */
    bool Init(const sdbus::string& name , int type , bool durable = true) ;

    const sdbus::string& Name() const ;
    void Name(const char * name) ;

    int Type() const  ;
    void Type(int type) ;

    bool Durable() const ;
    void Durable(bool durable) ;

    virtual void OnMsgs(const Message ** msgs , int count) ;

    inline void SetOptions(const Options& options) {options_ = options ;}
    inline const Options& GetOptions() const {return options_ ;}

    inline const Connection * Owner() const  {return connection_ ;}
    inline Connection * Owner() {return connection_ ;}

private:

    CallbackImpl * impl_ ;

    Options options_ ;
    Connection * connection_ ;
} ;

}

#endif  /**__SDBUS_CALLBACK_H */
