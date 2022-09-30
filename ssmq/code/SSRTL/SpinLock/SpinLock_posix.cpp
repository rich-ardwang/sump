

#include "SpinLock_posix.h"
#include "../SSLog.h"

namespace ss{
	
SpinLock::SpinLock()
{
	inited_ = (::pthread_spin_init(&locker_ , PTHREAD_PROCESS_PRIVATE) == 0);
    if(inited_ == false)
    {
        LOGEMERG("can not init pthread_spinlock_t") ;
    }
}

SpinLock::~SpinLock()
{
	::pthread_spin_destroy(&locker_) ;
}

bool SpinLock::lock()
{
	bool result = (inited_ == true) ? (::pthread_spin_lock(&locker_) == 0) : false ;	
    if(result == false)
    {
        LOGEMERG("spinlock lock failed") ;
    }
    return result ;
}

bool SpinLock::unlock()
{
	bool result = (inited_ == true) ? (::pthread_spin_unlock(&locker_) == 0) : false ;	
    if(result == false)
    {
        LOGEMERG("spinlock unlock failed") ;
    }
    return result ;    
}
	
	
}


