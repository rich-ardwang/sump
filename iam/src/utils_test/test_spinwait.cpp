//
//  test_spinwait.cpp
//
//  Created by Colin on 2019-07-04.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include "spin_wait.h"
#include "spin_lock.h"

using namespace lava::base;
using namespace lava::utils;

void spinwait_thread(spin_lock* lock)
{
	lava::utils::spin_wait swait; long long delta = 0;
	auto start_point = std::chrono::high_resolution_clock::now();
	do
	{
		auto end_point = std::chrono::high_resolution_clock::now();
		delta = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point).count();
		if (delta >= 20 * 1000)
			break;

		swait.yield();
	} while (1);
	
	std::lock_guard<spin_lock> guard(*lock);
	std::cout << "thread " << std::this_thread::get_id() << " takes " << delta << "ms to spin wait." << std::endl;
}

void test_spinwait()
{
	std::cout << "\nenter spin wait phase: about 20s in every thread." << std::endl;
	spin_lock lock;
	std::thread t1(spinwait_thread, &lock);
	std::thread t2(spinwait_thread, &lock);
	std::thread t3(spinwait_thread, &lock);
	std::thread t4(spinwait_thread, &lock);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	std::cout << "leave spin wait phase." << std::endl;
}