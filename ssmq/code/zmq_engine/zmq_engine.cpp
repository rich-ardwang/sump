// 这是主 DLL 文件。

#include "engine/zmq_engine.h"
#include "zconnection.h"

#include "SSRTL/SSLog.h"
#include "zmq/zmq.h"

namespace engine{

namespace zmq{

//     class global_ztx {
//     public:
//         global_ztx(){ context = zmq_ctx_new();}; 
//         ~global_ztx(){ 
//             if( NULL != context ) zmq_ctx_destroy (context); 
//         };
// 
//         static global_ztx& Singleton()
//         {
//             static global_ztx __global_ztx__ ;
//             return __global_ztx__;    
//         };
// 
//         void *ctx() { return context; };
//         void reinit()
//         {
//             if( NULL != context ) zmq_ctx_destroy (context);
//             context = zmq_ctx_new();
//             return;
//         };
// 
//     private:
//         void *context;
// };




void LogLevel(int level) 
{
    DEFAULT_LOGGER->set_level(level) ;
    //SetQpidLogLevel(level) ;
}

const char *Version()
{
    static const char* __version__ = "2.0.1.20150127";
    return __version__;
}


ZmqEngine::~ZmqEngine()
{
    if( NULL == PrivData ) return;
//     void *ctx = global_ztx::Singleton().ctx();
//     if( ctx ) zmq_ctx_destroy( ctx );
}
sdbus::Engine * ZmqEngineCreator() 
{
    sdbus::Engine *eg = new sdbus::Engine();

    if( NULL == eg) return NULL;
    
    eg->Creator     = ZmqEngineCreator;
    eg->Destroyer   = ZmqEngineDestroyer;

    eg->Initializer = ZmqEngineInitializer;
    eg->Finalizer   = ZmqEngineFinalizer;
    
    eg->AddHandler  = ZmqEngineAddHandler;
    eg->DelHandler  = ZmqEngineDelHandler;

    eg->AsyncSend   = ZmqEngineAsyncSend;
    eg->SyncSend    = ZmqEngineSyncSend;

    eg->Publish     = ZmqEnginePublish;

    eg->SetOptions  = ZmqEngineSetOptions;
    eg->GetOptions  = ZmqEngineGetOptions;
    return eg;
}

void ZmqEngineDestroyer(sdbus::Engine * engine) 
{
    //if( NULL == engine ) return;
    //if( NULL != engine->PrivData )
    //{
    //    zmq_close(engine->PrivData);
    //    engine->PrivData = NULL;
    //}
    delete engine;
    return;
}

bool ZmqEngineInitializer(sdbus::Engine * engine , const sdbus::string& url) 
{
    Zconnection *Zconn = NULL;
    if( NULL == engine ) return false;
    if( NULL != engine->PrivData ) return false;

//     void *ctx = global_ztx::Singleton().ctx();
//     if( NULL != ctx)
//     {
        Zconn = new Zconnection();
        if( NULL == Zconn ) return false;
        if( !Zconn->Connect( url.c_str() ) )
        {
            delete Zconn;
            engine->PrivData = NULL;
            return false;
        }

        engine->PrivData = (void *)Zconn;
        return true;
/*    }*/
    return false;
}
void ZmqEngineFinalizer(sdbus::Engine * engine) 
{
    if( NULL == engine)  return;
    if( NULL != engine->PrivData )
    {
        //zmq_close( engine->PrivData );
		Zconnection *Zconn = (Zconnection*)engine->PrivData;
		Zconn->Close();
		delete Zconn;
        engine->PrivData = NULL;
    }
    return;
}

bool ZmqEngineAddHandler(sdbus::Engine * engine , const sdbus::string& name , int type , sdbus::Callback * cb) 
{
    if(engine == NULL || engine->PrivData == NULL)  
        return false ;
    Zconnection * conn = (Zconnection *)engine->PrivData ;
    return conn->AddHandler(cb) ;
}
void ZmqEngineDelHandler(sdbus::Engine * engine , const sdbus::Callback * cb) 
{
    if(engine == NULL || engine->PrivData == NULL)  
        return  ;
    Zconnection * conn = (Zconnection *)engine->PrivData ;
    conn->DelHandler(cb) ;
    return;
}

bool ZmqEngineAsyncSend(sdbus::Engine * engine , const sdbus::string& name , const sdbus:: Message& msg) 
{
    if(engine == NULL || engine->PrivData == NULL)
        return false ;

    Zconnection * conn = (Zconnection *)engine->PrivData ;
    return conn->AsyncSend(name , msg , sdbus::DeliverDirect) ;
}
bool ZmqEngineSyncSend(sdbus::Engine * engine , const sdbus::string& name , const sdbus:: Message& request , sdbus::Message& response , int timeout) 
{
	if(engine == NULL || engine->PrivData == NULL)
		return false ;

	Zconnection * conn = (Zconnection *)engine->PrivData ;
	return conn->SyncSend(name , request ,response, timeout) ;

}

bool ZmqEnginePublish(sdbus::Engine * engine , const sdbus::string& topic , const sdbus::Message& msg) 
{
    if(engine == NULL || engine->PrivData == NULL)
        return false ;

    Zconnection * conn = (Zconnection *)engine->PrivData ;
    return conn->AsyncSend(topic , msg , sdbus::DeliverPublish ) ;
}

bool ZmqEngineSetOptions(sdbus::Engine * engine , const sdbus::Options& options) 
{
    return true;
}
const sdbus::Options& ZmqEngineGetOptions(sdbus::Engine * engine ) 
{
   //sdbus::Options tmp;
   return sdbus::Options::EMPTY;
}
ZmqEngine::ZmqEngine() 
{
    Creator = ZmqEngineCreator ;
    Destroyer = ZmqEngineDestroyer ;

    Initializer = ZmqEngineInitializer ;
    Finalizer = ZmqEngineFinalizer ;

    AddHandler = ZmqEngineAddHandler;
    DelHandler = ZmqEngineDelHandler;

    AsyncSend = ZmqEngineAsyncSend;
    SyncSend = ZmqEngineSyncSend;

    Publish = ZmqEnginePublish;

    SetOptions = ZmqEngineSetOptions;
    GetOptions = ZmqEngineGetOptions;

    PrivData = NULL ;
}

}//end zero


namespace zbroker
{
int create_broker( char *url,size_t len,size_t limit )
{
    return zbroker::create( url,len,limit);
}
}//end namespace zbroker



}//end engine