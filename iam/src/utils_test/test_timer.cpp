//
//  test_queue.cpp
//
//  Created by Colin on 2019-08-06.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <iostream>
#include <thread>
#include "timer.h"

using namespace lava::base;
using namespace lava::utils;

void timer_callback(const char* s)
{
	std::cout << "timer_callback : thread_id = " << std::this_thread::get_id() << ", " << s << std::endl;
}

void test_timer()
{
	std::cout << "test timer : thread_id = " << std::this_thread::get_id() << std::endl;

	timer t;
	t.start(1000, std::bind(timer_callback, "hello world!"));
	std::this_thread::sleep_for(std::chrono::seconds(4));
	std::cout << "try to expire timer!" << std::endl;
	t.expire();

	t.start(1000, std::bind(timer_callback, "hello c++11!"));
	std::this_thread::sleep_for(std::chrono::seconds(4));
	std::cout << "try to expire timer!" << std::endl;
	t.expire();

	std::this_thread::sleep_for(std::chrono::seconds(2));

	t.sync_wait(1000, timer_callback, "hello world!");
	t.async_wait(1000, timer_callback, "hello c++11!");

	std::this_thread::sleep_for(std::chrono::seconds(2));
}