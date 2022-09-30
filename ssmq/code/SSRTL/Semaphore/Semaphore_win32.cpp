
#include "Semaphore_win32.h"

namespace ss{

Semaphore::Semaphore()
{
    handle_ = ::CreateSemaphore(NULL , 1 , MAXLONG , NULL) ;
}

Semaphore::~Semaphore()
{
    if(handle_ != NULL)
    {
        ::CloseHandle(handle_) ;
        handle_ = NULL ;
    }
}

bool Semaphore::Wait()
{
    return TimedWait(-1) ;
}

bool Semaphore::TimedWait(int sec)
{
    DWORD timeout = 0 ;
    if(sec < 0)
        timeout = INFINITE ;        
    else
        timeout = (DWORD)(sec * 1000) ;

    DWORD result = ::WaitForSingleObject(handle_ , timeout) ;
    if(result == WAIT_OBJECT_0)
        return true ;
    else
        return false ;
}

bool Semaphore::Notify()
{
    return (::ReleaseSemaphore(handle_ , 1 , NULL)  == TRUE) ;
}

}
