#include <iostream>
#include <atomic>

std::atomic<bool> g_thread_exit_flag;

#ifdef LAVA_WIN
#include <windows.h> 
#include <stdio.h> 
BOOL WINAPI my_handler(DWORD fdwCtrlType)
{
	switch ( fdwCtrlType )
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		//printf("Ctrl-C event\n\n");
		//Beep(750, 300);
		g_thread_exit_flag.store(true);
		return TRUE;

		// CTRL-CLOSE: confirm that the user wants to exit. 
	//case CTRL_CLOSE_EVENT:
	//	Beep(600, 200);
	//	printf("Ctrl-Close event\n\n");
	//	return TRUE;

	//	// Pass other signals to the next handler. 
	//case CTRL_BREAK_EVENT:
	//	Beep(900, 200);
	//	printf("Ctrl-Break event\n\n");
	//	return FALSE;

	//case CTRL_LOGOFF_EVENT:
	//	Beep(1000, 200);
	//	printf("Ctrl-Logoff event\n\n");
	//	return FALSE;

	//case CTRL_SHUTDOWN_EVENT:
	//	Beep(750, 500);
	//	printf("Ctrl-Shutdown event\n\n");
	//	return FALSE;

	default:
		return FALSE;
	}
}
#else
#include <signal.h>
void my_handler(int s) 
{
	g_thread_exit_flag.store(true);
}
#endif