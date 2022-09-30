//
//  test_stack.cpp
//
//  Created by Colin on 2019-07-03.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <iostream>
#include <thread>
#include <mutex>
#include "spin_lock.h"

using namespace lava::base;
using namespace lava::utils;

static int node_count = 1000;

void spinlock_thread1(spin_lock* lock)
{
	for (int i = 0; i < node_count; i++)
	{
		std::lock_guard<spin_lock> guard(*lock);
		std::cout << "hello" << std::endl;
	}
}

void spinlock_thread2(spin_lock* lock)
{
	for (int i = 0; i < node_count; i++)
	{
		std::lock_guard<spin_lock> guard(*lock);
		std::cout << "world" << std::endl;
	}
}

void test_spinlock()
{
	std::cout << "\nenter spin lock phase." << std::endl;
	spin_lock lock;
	std::thread t1(spinlock_thread1, &lock);
	std::thread t2(spinlock_thread2, &lock);
	t1.join();
	t2.join();
	std::cout << "leave spin lock phase." << std::endl;
}