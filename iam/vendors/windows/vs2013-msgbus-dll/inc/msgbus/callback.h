
#ifndef __MSGBUS_CALLBACK_H
#define __MSGBUS_CALLBACK_H 1

#include "msgbus/message.h"

namespace msgbus {

enum DeliverType{
    DeliverDirect ,
    DeliverFanout ,
    DeliverBrowse 
} ;

typedef void (*HANDLER_CALLBACK)(const std::string &name, const Message &message, void *client_data);

class MSGBUSAPI Callback{
public:
    Callback() ;
    virtual ~Callback() {}

    inline const std::string& name() const {return name_ ;}
    inline void name(const std::string& str) {name_ = str ;}
    inline void * client_data() const {return client_data_ ;}
    inline void client_data(void * data) {client_data_ = data ;}

    inline HANDLER_CALLBACK handler() const {return handler_ ; }
    inline void handler(HANDLER_CALLBACK func) {handler_ = func ; }

    inline DeliverType type() const {return type_ ;}
    inline void type(DeliverType dt) {type_ = dt;}

    virtual void message(const Message& msg) ;

protected:
    void * client_data_ ;
    std::string name_ ;

    DeliverType type_ ;

    /*
        为了兼容旧版本，留下这个接口
    */
    HANDLER_CALLBACK handler_ ;
} ;

class MSGBUSAPI VectCallback : public Callback{
public:

    virtual void messages(const Message* msgs , int count) ;
} ;

}

#endif      /** __MSGBUS_CALLBACK_H */
