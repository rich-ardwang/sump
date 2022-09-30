
#ifndef __SS_SPIN_LOCK_WIN32_H
#define __SS_SPIN_LOCK_WIN32_H 1

#include "SSRTL/SSAtomic.h"

namespace ss{

class SpinLock{
public:
	SpinLock() ;
	~SpinLock() ;
	
	bool lock() ;
	bool unlock() ;
	
private:
	atomic_count counter_ ;
    long sequence_ ;
} ;	

}

#endif /** __SS_SPIN_LOCK_WIN32_H */