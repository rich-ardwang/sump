
#ifndef __SS_QUEUE_H
#define __SS_QUEUE_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/condition.hpp>

#include "SSRTL/SSSpinLock.h"
#include "SSRTL/SSSemaphore.h"
#include "SSRTL/SSThread.h"

namespace ss {

/*
    研究发现，使用无限下标，不需要dummy节点。因此可以满缓冲存放。
    因此，需要重算size。
*/
class Queue{
public:
    Queue() ;
    ~Queue() ;

    bool Init(size_t cap) ;
    void Final() ;
    void Clear() ;

    bool Push(void * data , int timeout = 0) ;
    bool TryPush(void * data) ;
    void * Pop(int timeout = 0) ;
    void * TryPop() ;

    int Pop(void ** datas , int count , int timeout = 0) ;

    inline size_t Size() const  {return tail_ - head_ ;}
    inline size_t Capacity() const {return capacity_ ;}    
    inline bool Full() const 
    {
        /*
            这个算法不能解决回绕问题，注意了。请看下面的不等式
        */
        //return tail_ + 1 >= head_ + capacity_ ;
        //return (tail_ - head_ + 1>= capacity_) ;
        return (tail_ - head_ >= capacity_) ;
    }
    inline bool Empty() const {return head_ == tail_ ;}

    inline size_t ENCounter() const {return encounter_ ;}
    inline size_t DNCounter() const {return dncounter_ ;}
    inline size_t EWTimes() const {return ewtimes_ ;}
    inline size_t DWTimes() const {return dwtimes_ ;}

    const void * Get(int idx) const ;    
    bool Extend(size_t cap) ;

    inline size_t Head() const {return head_ ;}
    inline size_t Tail() const {return tail_ ;}

private:
    void ** datas_ ;
    size_t capacity_ ;
    size_t head_ ;      //如果是非空，那么指向有效的节点
    size_t tail_ ;      //如果是非满，那么指向可用的节点。

    size_t encounter_ ;
    size_t dncounter_ ;

    size_t ewtimes_ ;
    size_t dwtimes_ ;

    bool finaled_ ;

    SpinLock elocker_ ;
    SpinLock dlocker_ ;

    bool ewaiting_ ;
    bool dwaiting_ ;

    /*
        发现thread_yield并不是有效的暂停办法，
    */
    boost::mutex mguard_ ;

    Semaphore esema_ ;
    Semaphore dsema_ ;
} ;

}

#endif  /** __SS_QUEUE_H */
