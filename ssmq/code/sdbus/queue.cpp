
#include "sdbus/queue.h"
#include "SSRTL/SSQueue.h"

namespace sdbus{

class QueueImpl : public ss::Queue{} ;

Queue::Queue()
{
    impl_ = new QueueImpl() ;
}

Queue::~Queue()
{
    if(impl_ != NULL)
    {
        impl_->Final() ;
        delete impl_ ;
        impl_ = NULL ;
    }
}
    
bool Queue::Init(size_t cap)
{
    return impl_->Init(cap) ;
}

void Queue::Final()
{
    impl_->Final() ;
}

void Queue::Clear()
{
    impl_->Clear() ;
}

bool Queue::Push(void * data , int timeout)
{
    return impl_->Push(data ,  timeout) ;
}

bool Queue::TryPush(void * data)
{
    return impl_->TryPush(data) ;
}

void * Queue::Pop(int timeout)
{
    return impl_->Pop(timeout) ;
}

void * Queue::TryPop()
{
    return impl_->TryPop() ;
}

int Queue::Pop(void **datas , int count , int timeout)
{
    return impl_->Pop(datas , count , timeout) ;
}

size_t Queue::Size() const
{
    return impl_->Size() ;
}

bool Queue::Full() const
{
    return impl_->Full() ;
}

bool Queue::Empty() const
{
    return impl_->Empty() ;
}

const void * Queue::Get(int idx) const
{
    return impl_->Get(idx) ;
}

bool Queue::Extend(size_t cap)
{
    return impl_->Extend(cap) ;
}

size_t Queue::Head() const
{
    return impl_->Head() ;
}

size_t Queue::Tail() const
{
    return impl_->Tail() ;
}

}
