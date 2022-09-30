#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <memory.h>

namespace sfa {
namespace msg {
namespace utils {

//字节流转换为十六进制字符串
char* ByteToHexStr(char* dest, const unsigned char* src, int src_len) {
	assert(dest != nullptr);
	assert(src != nullptr && src_len > 0);

	for (int i = 0; i < src_len; i++) {
		unsigned char high_byte = src[i] >> 4;
		unsigned char low_byte = src[i] & 0x0f;

		high_byte += 0x30;
		if (high_byte > 0x39) {
			dest[i * 2] = high_byte + 0x07;
		}
		else {
			dest[i * 2] = high_byte;
		}

		low_byte += 0x30;
		if (low_byte > 0x39) {
			dest[i * 2 + 1] = low_byte + 0x07;
		}
		else {
			dest[i * 2 + 1] = low_byte;
		}
	}
	return dest;
}

//字节流转换为十六进制字符串的另一种实现方式
std::string HexToStr(const std::string& src) {
	std::string dest("");
	for (size_t i = 0; i < src.length(); i++) {
		char ch[3] = { 0 };
		sprintf(ch, "%02X", (unsigned char)src.at(i));
		dest += ch;
	}
	return dest;
}

//十六进制字符串转换为字节流
unsigned char* HexStrToByte(unsigned char* dest, const char* src, int src_len) {
	assert(dest != nullptr);
	assert(src != nullptr && src_len > 0);

	for (int i = 0; i < src_len; i += 2) {
		unsigned char high_byte = toupper(src[i]);
		unsigned char low_byte = toupper(src[i + 1]);

		if (high_byte > 0x39) {
			high_byte -= 0x37;
		}
		else {
			high_byte -= 0x30;
		}

		if (low_byte > 0x39) {
			low_byte -= 0x37;
		}
		else {
			low_byte -= 0x30;
		}

		dest[i / 2] = (high_byte << 4) | low_byte;
	}
	return dest;
}

}
}
}
