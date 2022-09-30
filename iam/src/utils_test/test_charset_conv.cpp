//
//  test_charset_conv.cpp
//
//  Created by Colin on 2019-07-03.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include <iostream>
#include <chrono>
#include "lava_conv.h"

using namespace lava::base;

static int node_count = 1000000;

void sentence_conv(wchar_t* pw)
{
	LAVA_USES_CONVERSION_EX;
	char* p_utf8 = WCHAR_TO_UTF8_EX(pw);
	wchar_t* pw2 = UTF8_TO_WCHAR_EX(p_utf8);

	char* p_gbk = WCHAR_TO_GBK_EX(pw);
	wchar_t* pw3 = GBK_TO_WCHAR_EX(p_gbk);
}

void test_charset_conv()
{
	LAVA_USES_CONVERSION_EX;
	wchar_t* pw = L"森浦是全球最伟大的公司之一@:)";
#ifdef LAVA_WIN
	char* p_asc = WCHAR_TO_GBK_EX(pw);
#else
	char* p_asc = WCHAR_TO_UTF8_EX(pw);
#endif

	std::cout << "\nenter charset-conversion phase: test performance of the following sentence's conversion." << std::endl;
	std::cout << p_asc << std::endl;
	auto start_point = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < node_count; i++)
		sentence_conv(pw);

	auto end_point = std::chrono::high_resolution_clock::now();
	auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point).count();
	std::cout << "it takes " << delta << "ms to convert " << node_count * 4 << " times" << std::endl;
	std::cout << "leave charset-conversion phase." << std::endl;
}
