
#include "SSQueue.h"
#include "SSAtomic.h"

namespace ss{


Queue::Queue()
{
    datas_ = NULL ;
    capacity_ = 0 ;
    head_ = tail_ = 0 ;
    finaled_ = false ;

    encounter_ = dncounter_ = 0 ;
    ewtimes_ = dwtimes_ = 0 ;

    ewaiting_ = dwaiting_ = false ;
}

Queue::~Queue()
{
    Final() ;
}

bool Queue::Init(size_t cap)
{
    return Extend(cap) ;
}

void Queue::Final()
{
    finaled_ = true ;

    void ** tmp = NULL ;

    elocker_.lock() ;
    dlocker_.lock() ;

    tmp = datas_ ;
    datas_ = NULL ;

    head_ = tail_ = 0 ;
    capacity_ = 0 ;

    elocker_.unlock() ;
    dlocker_.unlock() ;
    
    boost::mutex::scoped_lock lock(mguard_) ;
    if(tmp != NULL)
    {
        ::free(tmp) ;
    }
}

void Queue::Clear()
{
    elocker_.lock() ;
    dlocker_.lock() ;

    head_ = tail_ = 0 ;

    dlocker_.unlock() ;
    elocker_.unlock() ;
}

bool Queue::TryPush(void * data) 
{
    elocker_.lock() ;
    if(Full() == true || datas_ == NULL)
    {
        elocker_.unlock() ;
        return false ;
    }

    datas_[tail_ % capacity_] = data ;
    ++tail_ ;
    elocker_.unlock() ;

    return true ;
}

bool Queue::Push(void * data , int timeout)
{
    if(timeout == 0)
        return TryPush(data) ;

    bool result = false ;
    if(datas_ == NULL)
        return result ;

    if(timeout < 0)
        timeout = 0x7FFFFFFF ;

    time_t begin = ::time(NULL) ;
    bool need_notify = false ;
    elocker_.lock() ;
    do{
        if(Full() == false)
        {
            result = true ;
            break ;
        }
        ewaiting_ = true ;
        ++ewtimes_ ;
        elocker_.unlock() ;

        esema_.TimedWait(1) ;

        time_t now = ::time(NULL) ;
        elocker_.lock() ;
        if((int)(now - begin) > timeout)
            break ;
    }while(finaled_ == false) ;

    ewaiting_ = false ;
    if(result == true)
    {
        size_t didx = tail_ % capacity_ ;

        /*
            在多核机器中，发现++tail_先完成，但读线程仍然读到旧的数据。
            因此必须加上内存屏障。
        */
        datas_[didx] = data ;
        ss::memory_barrier() ;
        ++tail_ ;

        if(dwaiting_ == true)
        {
            ++dncounter_ ;
            need_notify = true ;
            //dsema_.Notify() ;
        }
    }
    elocker_.unlock() ;

    if(need_notify == true)
    {
        dsema_.Notify() ;
    }

    return result ;
}

void * Queue::TryPop() 
{
    dlocker_.lock() ;
    if(Empty() == true)
    {
        dlocker_.unlock() ;
        return NULL ;
    }

    size_t hidx = head_ % capacity_;
    void * data = datas_[hidx] ;

    if(Full() == true)
        ++encounter_ ;
    ++head_ ;
    dlocker_.unlock() ;
    return data ;
}

int Queue::Pop(void ** datas , int count , int timeout) 
{
    if(timeout  == 0)
    {
        void * data = TryPop() ;
        if(data == NULL)
            return 0 ;
        datas[0] = data ;
        return 1 ;
    }

    bool result = false ;
    if(datas_ == NULL)
        return NULL ;

    if(timeout < 0)
        timeout = 0x7FFFFFFF ;

    time_t begin = ::time(NULL) ;   
    bool need_notify = false ;
    dlocker_.lock() ;
    do
    {
        if(Empty() == false)
        {
            result = true ;
            break ;
        }

        dwaiting_ = true ;
        ++dwtimes_ ;
        dlocker_.unlock() ;

        dsema_.TimedWait(1) ;

        time_t now = ::time(NULL) ;
        dlocker_.lock() ;
        if((int)(now - begin) > timeout)
            break ;
    }while(finaled_ == false) ;

    dwaiting_ = false ;

    int counter =  0 ;
    if(result == true)
    {
        size_t hidx = head_ % capacity_ ;
        size_t hcount = tail_ - head_ ;
        while((counter < count) && (counter < (int)hcount))
        {
            if(hidx >= capacity_)
                hidx = 0 ;

            datas[counter] = datas_[hidx] ;
            ++counter ;
            ++hidx ;
        }

        head_ += counter ;

        if(ewaiting_ == true)
        {
            ++encounter_ ;
            need_notify = true ;            
            //esema_.Notify() ;
        }
    }
    dlocker_.unlock() ;

    if(need_notify == true)
    {
        esema_.Notify() ;
    }

    return counter ;
}

void * Queue::Pop(int timeout)
{
    if(timeout == 0)
        return TryPop() ;

    void * datas[1] ;
    int count = Pop(datas , 1 , timeout) ;
    if(count == 0)
        return NULL ;
    else
        return datas[0] ;
}

const void * Queue::Get(int idx) const
{
    if(datas_ == NULL || idx < 0 || idx >= (int)capacity_)
        return NULL ;

    return datas_[idx] ;
}

bool Queue::Extend(size_t cap)
{
    if(cap <= capacity_)
        return true ;

    boost::mutex::scoped_lock locke(mguard_) ;

    size_t dsize = sizeof(void *) * cap ;
    void ** datas = (void **)::malloc(dsize) ;
    if(datas == NULL)
        return false ;
    ::memset(datas , 0 , dsize) ;

    if(datas_ != NULL)
    {
        size_t osize = sizeof(void *) * capacity_ ;
        if(osize > 0)
        {
            ::memcpy(datas , datas_ , osize) ;
        }
    }

    void ** tmp = datas_ ;

    elocker_.lock() ;
    dlocker_.lock() ;

    datas_ = datas ;
    capacity_ = cap ;

    dlocker_.unlock() ;
    elocker_.unlock() ;

    if(tmp != NULL)
        ::free(tmp) ;

    return true ;
}

}

