#include <iostream>
#include <string>
#include "lava_utils_api.h"

#define SHA256_OUTPUT_LENGTH	(32 * 2 + 1)

void test_sha256()
{
	std::string text;
	std::cout << "please input original text:\n";
	while (std::cin >> text)
	{
		char result_buf[SHA256_OUTPUT_LENGTH];
		int ret = sha256_encode(text.c_str(), text.length(), result_buf, SHA256_OUTPUT_LENGTH);
		if (0 == ret)
			std::cout << "out:" << result_buf << std::endl;
		else
			std::cout << "sha256 encode error, err_id:" << ret << std::endl;
		std::cout << "please input original text:\n";
	}
}
