//
//  lava_utils_api.h
//
//  Created by Colin on 2019-07-25.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once


#ifdef LAVA_WIN
#ifdef LAVA_UTILS_EXPORTS
#define lava_utils_api __declspec(dllexport)
#else
#define lava_utils_api __declspec(dllimport)
#endif
#elif defined (LAVA_MACOSX)
#define lava_utils_api __attribute__ ((visibility ("default")))
#else // LAVA_LINUX
#define lava_utils_api 
#endif

#include "lava_base.h"

#define lava_utils	"lava.lava_utils"
#define lava_comm	"lava.lava_comm"
#define lava_box	"lava.lava_box"

//extern "C"
//{};

lava_utils_api uint8_t* base64_encode(__lv_in uint8_t* orig, __lv_in uint32_t orig_len);
lava_utils_api int32_t  base64_encode(__lv_out uint8_t* result, __lv_in uint8_t* orig, __lv_in uint32_t orig_len);
lava_utils_api uint8_t* base64_decode(__lv_in uint8_t* src, __lv_out uint32_t& size, __lv_in bool trim_trailing_zero = true);
lava_utils_api void     free_base64_result(uint8_t* result);

// uuid
lava_utils_api char* get_uuid();
lava_utils_api void free_uuid(char * uuidptr);

// gzip
lava_utils_api int lava_gzip_string(__lv_in const char* source_buffer, __lv_in int source_length, __lv_out char* dest_buffer, __lv_out int& dest_length);
lava_utils_api int lava_ungzip_byte(__lv_in const char* source_buffer, __lv_in int source_length, __lv_out char* dest_buffer, __lv_out int& dest_length);

// log

typedef enum { Trace, Debug, Info, Warning, Error } LogLevel;
lava_utils_api void lava_log4(__lv_in const char* module, __lv_in LogLevel level, __lv_in const char* msgfmt, ...);
lava_utils_api void lava_log4(__lv_in const char* module, __lv_in LogLevel level, __lv_in const char* file, __lv_in const char* function, __lv_in const int line, __lv_in const char* msgfmt, ...);
#ifdef LAVA_WIN
	lava_utils_api void lava_log4(__lv_in const wchar_t* module, __lv_in LogLevel level, __lv_in const wchar_t* msgfmt, ...);
	lava_utils_api void lava_log4(__lv_in const wchar_t* module, __lv_in LogLevel level, __lv_in const wchar_t* file, __lv_in const wchar_t* function, __lv_in const int line, __lv_in const wchar_t* msgfmt, ...);
#endif

#define LOG(module, level, format, ...) lava_log4(module, level, format,##__VA_ARGS__)
#define log_trace(module, format, ...) LOG(module, Trace, format, ##__VA_ARGS__)
#define log_debug(module, format ,...) LOG(module, Debug, format, ##__VA_ARGS__)
#define log_info(module, format, ...) LOG(module, Info, format, ##__VA_ARGS__)
#define log_warn(module, format, ...) LOG(module, Warning, format, ##__VA_ARGS__)
#define log_error(module, format, ...) LOG(module, Error, format, ##__VA_ARGS__)