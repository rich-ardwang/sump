// crypto_test.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <iostream>
#include "sha256.h"


int _tmain(int argc, _TCHAR* argv[])
{
    SHA256 sha256;
    std::string text;
    std::cout << "please input original text:\n";
    while (std::cin >> text) {
        std::cout << sha256.generate(text) << std::endl;
        std::cout << "please input original text:\n";
    }

    std::cout << "enter to quit." << std::endl;
    std::cin.get();
	return 0;
}

