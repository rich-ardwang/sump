// utils_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "utils.h"

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

int _tmain(int argc, _TCHAR* argv[])
{
    /*
    std::cout << CUtils::getCurrentSystemTime() << std::endl;
    std::cout << CUtils::getCurrentSystemTime(time_format) << std::endl;
    std::cout << CUtils::getCurrentSystemTime(data_format1) << std::endl;
    std::cout << CUtils::getCurrentSystemTime(data_format2) << std::endl;
    */

    int nPid = (int)getpid();
    std::cout << "The process id is: " << nPid << std::endl;

    std::cout << "enter to quit." << std::endl;
    std::cin.get();
	return 0;
}

