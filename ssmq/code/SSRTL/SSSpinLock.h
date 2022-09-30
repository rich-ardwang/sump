
#ifndef __SS_SPIN_LOCK_H
#define __SS_SPIN_LOCK_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ))
	#include "SSRTL/SpinLock/SpinLock_posix.h"
#else
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__) || defined(MINGW) || defined(MINGW32)
		#include "SSRTL/SpinLock/SpinLock_win32.h"
    #else
        #error "spinlock not support"
	#endif
#endif


#endif  /** __SS_SPIN_LOCK_H */
