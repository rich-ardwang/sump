
#ifndef __SDBUS_ENGINE_H
#define __SDBUS_ENGINE_H 1


#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include "sdbus/options.h"

#include "sdbus/callback.h"
#include "sdbus/message.h"

namespace sdbus{

/*
    �����߲��棬���ܻ��в�ͬ��ѡ�͡����Ƕ���Ӧ�ÿ�������˵����Ҫһ���ȶ��Ľӿڣ�
    ͬʱ���κ���ľ���ʵ�֣�����Ӧ�ÿ�����Ӱ�졣
    ����sdbus��˵��Ӧ�ò�API��engine���Ž�ģʽ����ΪӦ�ò㲻ϣ���˽����ߵľ���ʵ�֡�
    ������engine������˵������ģʽ����Ҫ��ɲ�ͬ���ߵķ�װ��
*/
class SDBUSAPI Engine{
public:
    Engine() ;
    Engine(const Engine& engine);
    virtual ~Engine() ;
    Engine& operator=(const Engine& engine) {Assign(engine) ;return (*this) ;}

    Engine * (*Creator) ();
    void (*Destroyer)(Engine * engine) ;

    bool (*Initializer)(Engine * engine , const sdbus::string& url) ;
    void (*Finalizer)(Engine * engine) ;

    bool (*AddHandler)(Engine * engine , const sdbus::string& name , int type , Callback * cb) ;
    void (*DelHandler)(Engine * engine , const Callback * cb) ;

    bool (*AsyncSend)(Engine * engine , const sdbus::string& name , const Message& msg) ;
    bool (*SyncSend)(Engine * engine , const sdbus::string& name , const Message& request , Message& response , int timeout) ;

    bool (*Publish)(Engine * engine , const sdbus::string& topic , const Message& msg) ;    

    bool (*SetOptions)(Engine * engine , const Options& options) ;
    const Options& (*GetOptions)(Engine * engine ) ;

    void * PrivData ;

private:
    void Assign(const Engine& engine) ;
} ;


SDBUSAPI bool RegisterEngine(const sdbus::string& name , const Engine& engine) ;

SDBUSAPI bool UnregisterEngine(const sdbus::string& name) ;

SDBUSAPI Engine * CreateEngine(const sdbus::string& name) ;

SDBUSAPI void DestroyEngine(Engine * engine) ;


}

#endif /** __SDBUS_ENGINE_H */
