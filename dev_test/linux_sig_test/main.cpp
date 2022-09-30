#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>


bool g_exit = false;

void sys_sig_action(int nSig) {
	switch (nSig) {
	case SIGINT:
		std::cout << "received SIGINT signal, signal_id: " << nSig << std::endl;
		break;

	case SIGTERM:
		std::cout << "received SIGTERM signal, signal_id: " << nSig << std::endl;
		break;

	default:
		std::cout << "received unknown signal, signal_id: " << nSig << std::endl;
	}

	g_exit = true;
	std::cout << "leave sys_sig_action." << std::endl;
}

int main()
{
	signal(SIGINT, sys_sig_action);
	signal(SIGTERM, sys_sig_action);

    printf("hello from linux_sig_test!\n");

	while (true) {
		if (g_exit)
			break;

		std::cout << "wait exit..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}

	std::cout << "enter to quit." << std::endl;
	std::cin.get();
    return 0;
}