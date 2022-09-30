
#ifndef __SS_SEMAPHORE_POSIX_H
#define __SS_SEMAPHORE_POSIX_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

namespace ss{

class Semaphore{
public:
    Semaphore() ;
    ~Semaphore() ;

    bool Wait() ;
    bool TimedWait(int sec) ;
    bool Notify() ;

private:
    sem_t handle_ ;
} ;
}

#endif  /** __SS_SEMAPHORE_POSIX_H */
