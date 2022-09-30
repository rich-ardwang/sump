//
//  test_stack.cpp
//
//  Created by Colin on 2019-07-03.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <iostream>
#include <thread>
#include <mutex>
#include "stack.h"
#include "spin_lock.h"

using namespace lava::utils;
static int node_count = 1000000;

void consumer(stack<int>* c, spin_lock* lock)
{
	auto start_point = std::chrono::high_resolution_clock::now();
	int value = 0, count = 0;
	while (count != node_count)
	{
		bool ret = c->pop(value);
		if (ret)
		{
			std::cout << value << std::endl;
			++count;
		}
	}

	auto end_point = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point).count();
	{
		std::lock_guard<spin_lock> guard(*lock);
		std::cout << "stack consumer takes " << delta << "ms to consume " << node_count << " nodes" << std::endl;
	}
}

void producer(stack<int>* c, spin_lock* lock)
{
	auto start_point = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < node_count; i++)
	{
		c->push(i + 1);
	}

	auto end_point = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point).count();
	{
		std::lock_guard<spin_lock> guard(*lock);
		std::cout << "stack producer takes " << delta << "ms to produce " << node_count << " nodes" << std::endl;
	}
}


void test_stack()
{
	// 
	// WANING: This is a lock-free stack
	//
	std::cout << "\nenter stack test phase: test performance of lock-free stack." << std::endl;
	stack<int> c; spin_lock lock;
	std::thread t1(consumer, &c, &lock);
	std::thread t2(producer, &c, &lock);
	t1.join();
	t2.join();
	std::cout << "leave stack test phase." << std::endl;
}