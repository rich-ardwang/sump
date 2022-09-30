
#ifndef __SS_SEMAPHORE_H
#define __SS_SEMAPHORE_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )
	#  include "SSRTL/Semaphore/Semaphore_posix.h"
#else
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__) || defined(MINGW) || defined(MINGW32)
		#include "SSRTL/Semaphore/Semaphore_win32.h"
	#endif
#endif


#endif  /** __SS_SEMAPHORE_H */
