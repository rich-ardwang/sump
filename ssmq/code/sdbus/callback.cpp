
#include "sdbus/callback.h"
#include "sdbus/connection.h"
#include "impl/callback_impl.h"

namespace sdbus{


CallbackImpl::CallbackImpl()
{
    type_ = 0 ;
    durable_ = true; 
}

CallbackImpl::~CallbackImpl()
{
    //
}

const sdbus::string& CallbackImpl::Name() const 
{
    boost::shared_lock<boost::shared_mutex> lock(guard_) ;
    return name_ ;
}

void CallbackImpl::Name(const char * name)
{
    if(name == NULL)
        return ;

    boost::unique_lock<boost::shared_mutex> lock(guard_) ;
    name_ = name ;
}

Callback::Callback()
{
    impl_ = new CallbackImpl() ;
    connection_ = NULL ;
}

Callback::~Callback()
{
    if(impl_ != NULL)
    {
        delete impl_ ;
        impl_ = NULL ;
    }
}

bool Callback::Init(const sdbus::string& name , int type , bool durable)
{
    impl_->Name(name.c_str()) ;
    impl_->Type(type) ;
    impl_->Durable(durable) ;

    return true ;
}

const sdbus::string& Callback::Name() const 
{
    return impl_->Name() ;
}

void Callback::Name(const char * name)
{
    impl_->Name(name) ;
}

int Callback::Type() const 
{
    return impl_->Type() ;
}

void Callback::Type(int type) 
{
    impl_->Type(type) ;
}

bool Callback::Durable() const
{
    return impl_->Durable() ;
}

void Callback::Durable(bool durable)
{
    impl_->Durable(durable) ;
}


void Callback::OnMsgs(const Message ** msgs , int count)
{
    //
}

}
