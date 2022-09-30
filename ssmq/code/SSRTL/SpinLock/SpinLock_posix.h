
#ifndef __SS_SPIN_LOCK_POSIX_H
#define __SS_SPIN_LOCK_POSIX_H 1


#include <pthread.h>

namespace ss{
	
class SpinLock{
public:
	SpinLock() ;
	~SpinLock() ;
	
	bool lock() ;
	bool unlock() ;
	
private:
	pthread_spinlock_t locker_ ;
    bool inited_ ;
} ;	
	
}

#endif  /** __SS_SPIN_LOCK_POSIX_H */
