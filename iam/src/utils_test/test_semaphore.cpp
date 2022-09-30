#include <iostream>
#include <thread>
#include <mutex>
#include "queue.h"
#include "spin_lock.h"

using namespace lava::base;
using namespace lava::utils;

volatile bool			g_pause;
semaphore				g_pause_sem(0);
semaphore				g_queue_sem(0);

void pause() {
	g_pause = true;
	g_queue_sem.signal();
	g_pause_sem.wait();
}

void read() {
	while (true) {
		g_queue_sem.wait();

		if (g_pause) {
			g_pause_sem.signal();
			break;
		}
	}
}

void test_semaphore()
{
	// 
	// WANING: This is a lock-free queue that supports 
	// single-thread read and single-thread-write. so 
	// if you want multiple thread to read or write, you
	// need to synchronize these read/write threads
	//
	//bool syn_mode = true;
	std::cout << "\nenter queue test phase: test performance of lock-free queue." << std::endl;
	//queue_t<int> q1(syn_mode); spin_lock lock;

	g_pause = false;
	g_pause_sem.reset();
	g_queue_sem.reset();
	std::thread t1(read);
	std::thread t2(pause);
	
	t1.join();
	t2.join();
	std::cout << "leave queue test phase." << std::endl;
}