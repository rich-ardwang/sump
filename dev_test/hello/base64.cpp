//
//  base64.cpp
//
//  Re-Edited by lei.wang on 2019-08-09.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include "stdafx.h"
#include "base64.h"
#include <cstring>


namespace sfa {
namespace utils {

static const char base64_char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static char base64_decode_table[256];

static void init_base64_decode_table() {
	for (int i = 0; i < 256; ++i) {
		base64_decode_table[i] = (char)0x80;
	}

	for (int i = 'A'; i <= 'Z'; ++i) {
		base64_decode_table[i] = 0 + (i - 'A');
	}

	for (int i = 'a'; i <= 'z'; ++i) {
		base64_decode_table[i] = 26 + (i - 'a');
	}

	for (int i = '0'; i <= '9'; ++i) {
		base64_decode_table[i] = 52 + (i - '0');
	}

	base64_decode_table[(unsigned char)'+'] = 62;
	base64_decode_table[(unsigned char)'/'] = 63;
	base64_decode_table[(unsigned char)'='] = 0;
}

uint8_t* base64_decode(_sfa_in_ uint8_t* src, _sfa_out_ uint32_t& size, _sfa_in_ bool trim_trailing_zero/*= true*/) {
	if (nullptr == src) {
		return nullptr;
	}

	static bool init_decode_table = false;
	if (!init_decode_table) {
		init_base64_decode_table();
		init_decode_table = true;
	}

	uint32_t len = strlen((char*)src);
	uint8_t* out = new uint8_t[len + 1];
	int k = 0;
	int const j_max = len - 3;
	for (int j = 0; j < j_max; j += 4) { // in case "in" is not a multiple of 4 bytes (although it should be)
		char in_tmp[4], out_tmp[4];
		for (int i = 0; i < 4; ++i) {
			in_tmp[i] = src[i + j];
			out_tmp[i] = base64_decode_table[(unsigned char)in_tmp[i]];
			if ((out_tmp[i] & 0x80) != 0) {
				out_tmp[i] = 0; // pretend the input was 'A'
			}
		}

		out[k++] = (out_tmp[0] << 2) | (out_tmp[1] >> 4);
		out[k++] = (out_tmp[1] << 4) | (out_tmp[2] >> 2);
		out[k++] = (out_tmp[2] << 6) | out_tmp[3];
	}

	if (trim_trailing_zero) {
		while ( k > 0 && out[k - 1] == '/0' ) --k;
	}
	size = k;
	uint8_t* result = new uint8_t[size];
	memmove(result, out, size);
	delete [] out;
	return result;
}

uint8_t* base64_encode(_sfa_in_ uint8_t* orig, _sfa_in_ uint32_t orig_len) {
	if (nullptr == orig) {
		return nullptr;
	}

	unsigned const num_24bits = orig_len / 3;
	bool have_pad = orig_len > num_24bits * 3;
	bool have_padding2 = orig_len == num_24bits * 3 + 2;
	uint32_t num_result_bytes = 4 * (num_24bits + have_pad);
	uint8_t* result = new uint8_t[num_result_bytes + 1];

	// Map each full group of 3 input bytes into 4 output base-64 characters:
	uint32_t i;
	for (i = 0; i < num_24bits; ++i) {
		result[4 * i + 0] = base64_char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64_char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64_char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64_char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)
	if (have_pad) {
		result[4 * i + 0] = base64_char[(orig[3 * i] >> 2) & 0x3F];
		if (have_padding2) {
			result[4 * i + 1] = base64_char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64_char[(orig[3 * i + 1] << 2) & 0x3F];
		}
		else {
			result[4 * i + 1] = base64_char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
		result[4 * i + 4] = '\0';
	}
	else {
		result[4 * i + 0] = '\0';
	}

	return result;
}

int32_t base64_encode(_sfa_out_ uint8_t* result, _sfa_in_ uint8_t* orig, _sfa_in_ uint32_t orig_len) {
	if (nullptr == orig) {
		return -1;
	}

	unsigned const num_24bits = orig_len / 3;
	bool have_pad = orig_len > num_24bits * 3;
	bool have_padding2 = orig_len == num_24bits * 3 + 2;
	unsigned const num_result_bytes = 4 * (num_24bits + have_pad);

	// Map each full group of 3 input bytes into 4 output base-64 characters:
	uint32_t i;
	for (i = 0; i < num_24bits; ++i) {
		result[4 * i + 0] = base64_char[(orig[3 * i] >> 2) & 0x3F];
		result[4 * i + 1] = base64_char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
		result[4 * i + 2] = base64_char[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
		result[4 * i + 3] = base64_char[orig[3 * i + 2] & 0x3F];
	}

	// Now, take padding into account.  (Note: i == numOrig24BitValues)
	if (have_pad) {
		result[4 * i + 0] = base64_char[(orig[3 * i] >> 2) & 0x3F];
		if (have_padding2) {
			result[4 * i + 1] = base64_char[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
			result[4 * i + 2] = base64_char[(orig[3 * i + 1] << 2) & 0x3F];
		}
		else {
			result[4 * i + 1] = base64_char[((orig[3 * i] & 0x3) << 4) & 0x3F];
			result[4 * i + 2] = '=';
		}
		result[4 * i + 3] = '=';
		result[4 * i + 4] = '\0';
	}
	else {
		result[4 * i + 0] = '\0';
	}

	result[num_result_bytes] = '\0';
	return num_result_bytes;
}

void free_base64_result(_sfa_in_ uint8_t* result) {
	delete [] result;
}

}
}
