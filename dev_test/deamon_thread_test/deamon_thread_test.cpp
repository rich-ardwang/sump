// deamon_thread_test.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include <string>
#include "stdafx.h"
#include "thread_watch.h"


int _tmain(int argc, _TCHAR* argv[])
{
	lava::StackWalker::Instance();
	std::cout << "this is main." << std::endl;
	_asm   int   3   //只是为了让程序崩溃
	return 0;
}

