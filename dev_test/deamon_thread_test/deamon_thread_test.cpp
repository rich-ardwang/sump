// deamon_thread_test.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <iostream>
#include <string>
#include "stdafx.h"
#include "thread_watch.h"


int _tmain(int argc, _TCHAR* argv[])
{
	lava::StackWalker::Instance();
	std::cout << "this is main." << std::endl;
	_asm   int   3   //ֻ��Ϊ���ó������
	return 0;
}

