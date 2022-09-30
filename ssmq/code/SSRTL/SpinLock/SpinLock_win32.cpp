

#include "SpinLock_win32.h"
#include "../SSThread.h"

namespace ss{

SpinLock::SpinLock()
{
    sequence_ = 1 ;
}

SpinLock::~SpinLock()
{
    //
}
	
bool SpinLock::lock()
{
    long v = counter_.increment() ;
    int times = 0 ;
    while(v != sequence_)
    {
        ++times ;
        if(times == 4096)
        {
            times = 0 ;
            thread_yield() ;
        }
    }

    return true ;
}

bool SpinLock::unlock()
{
    ++sequence_ ;
    return true ;
}	

}

