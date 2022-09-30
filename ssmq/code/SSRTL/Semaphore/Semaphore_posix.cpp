
#include "Semaphore_posix.h"

namespace ss{
	
Semaphore::Semaphore()
{
	::sem_init(&handle_ , 0 , 1) ;
}

Semaphore::~Semaphore()
{
	::sem_destroy(&handle_) ;
}	

bool Semaphore::Wait()
{
	return (::sem_wait(&handle_) == 0) ;
}

bool Semaphore::TimedWait(int sec)
{
    if(sec == 0)
        return true ;
    else if(sec < 0)
    {
        return (::sem_wait(&handle_) == 0) ;
    }
        
	struct timespec  ts ;
	
	ts.tv_sec = ::time(NULL) + sec ;
	ts.tv_nsec = 0 ;
	
	return (::sem_timedwait(&handle_ , &ts) == 0) ;
	
}

bool Semaphore::Notify()
{
	return (::sem_post(&handle_) != 0) ;
}		
	
}

