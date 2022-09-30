//
//  base64.h
//
//  Re-Edited by lei.wang on 2019-08-09.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include <sfa/sfa_macro.h>

namespace sfa {
namespace utils {

uint8_t* base64_encode(_sfa_in_ uint8_t* orig, _sfa_in_ uint32_t orig_len);
int32_t base64_encode(_sfa_out_ uint8_t* result, _sfa_in_ uint8_t* orig, _sfa_in_ uint32_t orig_len);
uint8_t* base64_decode(_sfa_in_ uint8_t* src, _sfa_out_ uint32_t& size, _sfa_in_ bool trim_trailing_zero = true);
void free_base64_result(_sfa_in_ uint8_t* result);

}
}