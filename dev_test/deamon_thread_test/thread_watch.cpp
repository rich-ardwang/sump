#include "thread_watch.h"


namespace lava
{
#ifdef _WIN32

#define MODULENAME  "Deamon"

//stack walker
LPTOP_LEVEL_EXCEPTION_FILTER StackWalker::_PreviousFilter;
StackWalker *StackWalker::_Instance = NULL;

StackWalker::StackWalker()
{
	//LOG4CXX_DEBUG(GETLOGGER("StackWalker"), "Daemon starting");
	_PreviousFilter = NULL;
	_PreviousFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);
}

StackWalker::~StackWalker()
{
	SetUnhandledExceptionFilter(_PreviousFilter);
	_PreviousFilter = NULL;
}

void WINAPI StackWalker::StackWalk(CONST LPCONTEXT pCtx)
{
	HANDLE hProcess;
	hProcess = GetCurrentProcess();

	if (!SymInitialize(GetCurrentProcess(), NULL, TRUE))
	{
		//LOG4CXX_ERROR(GETLOGGER("StackWalker"), "SymInitialize failed. MODULENAME: " << MODULENAME << " Error: " << GetLastError());
		return;
	}

	//SEND_EAGLE_ERROR_INFO(500, "½ø³Ì±ÀÀ£", "StackWalker::StackWalk()", "");
	//LOG4CXX_ERROR(GETLOGGER("StackWalker"), "Process crashed, stack:");

	STACKFRAME64 sf64;
	ZeroMemory(&sf64, sizeof(sf64));

	DWORD dwMachineType;

	sf64.AddrPC.Mode = sf64.AddrFrame.Mode = sf64.AddrStack.Mode = AddrModeFlat;

#ifdef _M_IX86
	dwMachineType = IMAGE_FILE_MACHINE_I386;
	sf64.AddrPC.Offset = pCtx->Eip;
	sf64.AddrFrame.Offset = pCtx->Ebp;
	sf64.AddrStack.Offset = pCtx->Esp;
#elif _M_X64
	dwMachineType = IMAGE_FILE_MACHINE_AMD64;
	sf64.AddrPC.Offset = pCtx->Rip;
	sf64.AddrFrame.Offset = pCtx->Rsp;
	sf64.AddrStack.Offset = pCtx->Rsp;
#elif _M_IA64
	dwMachineType = IMAGE_FILE_MACHINE_IA64;
	sf64.AddrPC.Offset = pCtx->StIIP;
	sf64.AddrFrame.Offset = pCtx->IntSp;
	sf64.AddrBStore.Offset = pCtx->RsBSP;
	sf64.AddrStack.Offset = pCtx->IntSp;
#else
#error "Platform not supported!"
#endif

	while (true)
	{
		if (!::StackWalk64(dwMachineType, hProcess, NULL, &sf64, (LPVOID)pCtx, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
		{
			//LOG4CXX_ERROR(GETLOGGER("StackWalker"), "MODULENAME: " <<  MODULENAME  << " GetLastError: " << GetLastError());
			break;
		}
		if (sf64.AddrStack.Offset == 0)
			break;

		unsigned __int64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(unsigned __int64) - 1) / sizeof(unsigned __int64)];
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
		pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSymbol->MaxNameLen = MAX_SYM_NAME;

		DWORD64 dwDisplacement = 0;
		//if (SymFromAddr(hProcess, sf64.AddrPC.Offset, &dwDisplacement, pSymbol))
			//LOG4CXX_ERROR(GETLOGGER("StackWalker"), "[" << MODULENAME << "]" << (LPCVOID)sf64.AddrPC.Offset << "$p\t" << (LPCVOID)sf64.AddrFrame.Offset << "$p\t" << pSymbol->Name << "$hs+0x" << dwDisplacement << "$X");
		//else
			//LOG4CXX_ERROR(GETLOGGER("StackWalker"), "[" << MODULENAME << "]" << (LPCVOID)sf64.AddrPC.Offset << "$p\t" << (LPCVOID)sf64.AddrFrame.Offset << "$p\tSymFromAdd failed! LastErrorCode:" << GetLastError());
	}
#ifdef SHOWCRASHDIALOG
	//ÇÐ»»×ÊÔ´¾ä±ú
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//ÏÔÊ¾µÇÂ¼¶Ô»°¿ò
	CDialogCrashTip dlg;
	if (IDOK == dlg.DoModal() && dlg.IsIgnore())
	{
		//Log4cxx(LOG4CXX__FATAL, MODULENAME, _T("ºöÂÔµ±Ç°´íÎó"));
		return;
	}
	if (IDOK != dlg.DoModal() || !dlg.IsRestart())
	{
		ExitProcess(-1);
		return;
	}

#endif
	//LOG4CXX_ERROR(GETLOGGER("StackWalker"), "Process Crashed");

	ExitProcess(-1);
}

LONG WINAPI StackWalker::UnhandledExceptionFilter(CONST PEXCEPTION_POINTERS pExceptionInfo)
{
	StackWalk(pExceptionInfo->ContextRecord);
	return EXCEPTION_CONTINUE_SEARCH;
}
#endif

}
