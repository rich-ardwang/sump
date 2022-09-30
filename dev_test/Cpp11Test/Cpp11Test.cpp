// Cpp11Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

using namespace std;

auto add(int x, int y) -> int {
	return x + y;
}

int main()
{
	cout << "4+5=" << add(4, 5) << endl; 
}

