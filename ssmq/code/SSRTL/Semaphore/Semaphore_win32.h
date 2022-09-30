
#ifndef __SS_SEMAPHORE_WIN32_H
#define __SS_SEMAPHORE_WIN32_H 1

#include <windows.h>

namespace ss{

class Semaphore{
public:
    Semaphore() ;
    ~Semaphore() ;

    bool Wait() ;
    bool TimedWait(int sec) ;
    bool Notify() ;

private:
    HANDLE handle_ ;
} ;
}

#endif  /** __SS_SEMAPHORE_WIN32_H */
