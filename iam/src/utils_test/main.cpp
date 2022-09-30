//
//  main.cpp
//
//  Created by Colin on 2019-06-14.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <iostream>

void test_spinlock();
void test_spinwait();
void test_stack();
void test_queue();
void test_charset_conv();
void test_log();
void test_timer();

//void test_zookeeper();
void test_gzip();
void test_semaphore();

void test_lava_config();
void test_sha256();
void test_rwlock();

int main(int argc, char* argv[])
{
	//test_spinlock();
	//test_spinwait();
	//test_stack();
	//test_queue();
	//test_charset_conv();

	//test_log();
	//test_timer();
	//test_zookeeper();
	//test_gzip();
	//test_semaphore();

	test_lava_config();
	//test_sha256();
	//test_rwlock();

	std::cout << "\nPress enter key to exit." << std::endl;
	std::cin.get();
	return 0;
}