#pragma once

#ifdef _sfa_in_
#undef _sfa_in_
#define _sfa_in_
#else
#define _sfa_in_
#endif

#ifdef _sfa_out_
#undef _sfa_out_
#define _sfa_out_
#else
#define _sfa_out_
#endif

#ifdef _sfa_inout_
#undef _sfa_inout_
#define _sfa_inout_
#else
#define _sfa_inout_
#endif

#ifdef SFA_BUILD_DLL
#	ifdef SFA_EXPORT
#		if defined(_MSC_VER)
#			define SFA_API __declspec(dllexport)
#       else
#			define SFA_API
#       endif 
#	else
#		if defined(_MSC_VER)
#			define SFA_API __declspec(dllimport)
#       else
#			define SFA_API
#       endif 
#	endif
#else
#	define SFA_API 
#endif