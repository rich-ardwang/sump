
#include "SSRTL/SSThread.h"


#if defined(WIN32) || defined(_WINDOWS)
#include <windows.h>

namespace ss {

	uint32 current_thread_id()
	{
        return (uint32) ::GetCurrentThreadId() ;
	}

    void thread_yield()
    {
        ::SwitchToThread() ;
    }

    void thread_msleep(int msec)
    {
        ::Sleep((unsigned long)(msec)) ;
    }

    void thread_sleep(int sec) 
    {
        ::Sleep(sec * 1000) ;
    }
}

#else

#include <syscall.h>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>

namespace ss{

	uint32 current_thread_id()
	{
        return (uint32_t) syscall(__NR_gettid) ;
	}

    void thread_yield()
    {
        ::sched_yield() ;
    }

    void thread_msleep(int msec)
    {
        ::usleep(msec * 1000) ;
    }

    void thread_sleep(int sec) 
    {
        ::sleep(sec) ;
    }
}

#endif


