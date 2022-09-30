
#include "sdbus/connection.h"
#include "sdbus/engine.h"
#include "SSRTL/SSLog.h"

namespace sdbus{

Connection::Connection()
{
    engine_ = NULL ;
}

Connection::~Connection()
{
    Engine * eng = engine_ ;
    engine_ = NULL ;
    if(eng != NULL)
    {
        DestroyEngine(eng) ;
    }
}

bool Connection::Connect(const sdbus::string& url , const sdbus::string& engine) 
{
    if(engine_ != NULL)
    {
        DestroyEngine(engine_) ;
    }

    engine_ = CreateEngine(engine) ;
    if(engine_ == NULL)
        return false ;

    return engine_->Initializer(engine_ , url) ;
}

void Connection::Close()
{
    engine_->Finalizer(engine_) ;
}

bool Connection::AddHandler(Callback * cb)
{
    if(engine_ == NULL || cb == NULL)
        return false ;

    return engine_->AddHandler(engine_ , cb->Name() , cb->Type() , cb) ;
}

bool Connection::AddHandler(const sdbus::string& name , int type , Callback * cb)
{
    if(cb == NULL)
        return false;
    cb->Name(name.c_str()) ;
    cb->Type(type) ;

    return (engine_ != NULL) ? engine_->AddHandler(engine_ , name , type , cb) : false ;
}

void Connection::DelHandler(const Callback * cb) 
{
    if(engine_ == NULL)
        return ;
    engine_->DelHandler(engine_ , cb) ;
}

bool Connection::Send(const sdbus::string& name , const Message& msg)
{
    LOGNOTICE("sdbus will send a message type[%s] msgid[%s] replyto[%s] to queue[%s]" , 
        msg.GetType().c_str() , msg.GetMessageID().c_str() , msg.GetReplyTo().c_str() , name.c_str()) ;
    return (engine_ != NULL) ? engine_->AsyncSend(engine_ , name , msg) : false;
}

bool Connection::Send(const sdbus::string& name , const Message& request , Message& response , int timeout)
{
    return (engine_ != NULL) ? engine_->SyncSend(engine_ , name , request , response , timeout) : false;
}

bool Connection::Publish(const sdbus::string& topic , const Message& msg)
{
    LOGNOTICE("sdbus will publish a message type[%s] msgid[%s] replyto[%s] to topic[%s]" , 
        msg.GetType().c_str() , msg.GetMessageID().c_str() , msg.GetReplyTo().c_str() , topic.c_str()) ;

    return (engine_ != NULL) ? engine_->Publish(engine_ , topic , msg) : false;
}

bool Connection::SetOptions(const Options& options) 
{
    return (engine_ != NULL) ? engine_->SetOptions(engine_ , options) : false;
}

const Options& Connection::GetOptions() const
{
    return (engine_ != NULL) ? engine_->GetOptions(engine_) : Options::EMPTY ;
}

void Connection::LogLevel(int level)
{
    DEFAULT_LOGGER->set_level(level) ;
}

int Connection::LogLevel()
{
    return DEFAULT_LOGGER->get_level() ;
}

}
