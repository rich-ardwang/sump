#include <iostream>
#include <chrono>
#include "lava_utils_api.h"
#include <thread>
#include <vector>
#include <memory>
#include <atomic>

std::atomic<bool> g_thread_exit_flag;
#ifdef LAVA_WIN
#include <windows.h> 
#include <stdio.h> 
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
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
void my_handler(int s) {
	g_thread_exit_flag.store(true);
}
#endif
static int log_count = 1000000;

void producelog(const std::string & modulename)
{
	log_trace(modulename.c_str(), "producelog start");
	auto start_point = std::chrono::high_resolution_clock::now();
	std::string name = modulename;
	for (int i = 0; i < log_count; i++) 
	{
		if (i % 3 == 0)
			log_info(name.c_str(), "Test :%05d", i);
		else if (i % 17 == 0)
			log_debug(name.c_str(), "Test :%05d", i);
		else if (i % 431 == 0)
			log_error(name.c_str(), "Test :%05d", i);
		else if (i % 11 == 0)
			log_warn(name.c_str(), "Test :%05d", i);
		else if (i % 113 == 0) {
#ifdef LAVA_WIN
			lava_log4(L"utils_test.lava.sumscope", LogLevel::Error, L"simple unicode log :%05d", i);
#else
			lava_log4("utils_test.lava.sumscope", LogLevel::Error, "simple log :%05d", i);
#endif
		}
		else 
			log_trace(name.c_str(), "Test :%05d", i);
		if (i % 1000 == 0) 
		{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			std::this_thread::yield();

		}


		bool bExit = g_thread_exit_flag.load();
		if (bExit)
			break;
	}
	auto end_point = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point).count();
	{
		//std::cout << name.c_str()<<" log takes " << delta << "ms to consume " << log_count << std::endl;
	}
}

void test_log() {

#ifdef LAVA_WIN
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
#else
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
#endif
	g_thread_exit_flag.store(false);
	std::vector<std::unique_ptr<std::thread> > threads;
	for (int i = 0; i < 10; i++) 
	{
		if (i % 2 == 0)
			threads.push_back(std::unique_ptr<std::thread>(new std::thread(producelog, "utils_test.lava.sumscope")));
		else
			threads.push_back(std::unique_ptr<std::thread>(new std::thread(producelog, "utils_test.lava.sumscope")));
	}

	for (int i = 0; i < 10; i++) 
	{
		threads[i]->join();
	}
}