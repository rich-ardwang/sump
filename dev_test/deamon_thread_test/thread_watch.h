/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : thread_watch.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-11
*Desc        : the interface used for watching thread,
*            : when server broke down, it can start it again.
*History     : Add by Lei.Wang 2019-10-11
***************************************************************************************************/
#ifndef __THREAD_WATCH_H__
#define __THREAD_WATCH_H__

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#endif

namespace lava
{

#ifdef _WIN32
class StackWalker
{
public:
	static StackWalker *Instance()
	{
		if (!_Instance)
		{
			_Instance = new StackWalker();
		}
		return _Instance;
	}

private:
	StackWalker();
	~StackWalker();

private:
	static VOID WINAPI StackWalk(CONST LPCONTEXT pCtx);
	static LONG WINAPI UnhandledExceptionFilter(CONST PEXCEPTION_POINTERS pExceptionInfo);

private:
	static LPTOP_LEVEL_EXCEPTION_FILTER _PreviousFilter;
	static StackWalker *_Instance;
};
#endif

}

#endif	/* __THREAD_WATCH_H__ */
