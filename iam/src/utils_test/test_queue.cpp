//
//  test_queue.cpp
//
//  Created by Colin on 2019-07-03.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <iostream>
#include <thread>
#include <mutex>
#include "queue.h"
#include "spin_lock.h"

using namespace lava::base;
using namespace lava::utils;

static int node_count = 10000;
static semaphore s_sem(0);

void consumer(i_queue<int>* c, spin_lock* lock, bool sync_mode)
{
	auto start_point = std::chrono::high_resolution_clock::now(); // auto = std::chrono::time_point<std::chrono::high_resolution_clock>
	int value = 0;
	while (value != node_count)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		bool ret = c->read(&value);
		if (false == sync_mode && false == ret)
			s_sem.wait();
		//std::cout << value << std::endl;
	}

	auto end_point = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point).count();
	{
		std::lock_guard<spin_lock> guard(*lock);
		std::cout << "queue consumer takes " << delta << "ms to consume " << node_count << " nodes" << std::endl;
	}
}

void producer(i_queue<int>* c, spin_lock* lock, bool sync_mode)
{
	auto start_point = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < node_count; i++)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		c->write(i + 1, true);
		bool ret = c->flush();
		if (false == sync_mode && false == ret)
			s_sem.signal();
	}

	auto end_point = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point).count();
	{
		std::lock_guard<spin_lock> guard(*lock);
		std::cout << "queue producer takes " << delta << "ms to produce " << node_count << " nodes" << std::endl;
	}
}

void test_queue()
{
	// 
	// WANING: This is a lock-free queue that supports 
	// single-thread read and single-thread-write. so 
	// if you want multiple thread to read or write, you
	// need to synchronize these read/write threads
	//
	bool syn_mode = true;
	std::cout << "\nenter queue test phase: test performance of lock-free queue." << std::endl;
	queue_t<int> q1(syn_mode); spin_lock lock;
	std::thread t1(consumer, &q1, &lock, syn_mode);
	std::thread t2(producer, &q1, &lock, syn_mode);
	t1.join();
	t2.join();
	std::cout << "leave queue test phase." << std::endl;
}