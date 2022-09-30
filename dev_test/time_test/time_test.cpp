// time_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>


int _tmain(int argc, _TCHAR* argv[])
{
    while (true)
    {
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
        auto epoch = start.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::seconds>(epoch);
        int64_t duration = value.count();
        std::cout << duration << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    /*
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << int(duration.count()) << std::endl;
    */
    std::cout << "enter to quit." << std::endl;
    std::cin.get();
	return 0;
}

