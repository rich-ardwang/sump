//
//  sfa_log.h
//
//  Re-Edited by lei.wang on 2019-08-09.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include <sfa/sfa_macro.h>


namespace sfa {
namespace utils {

typedef enum { Trace, Debug, Info, Warning, Error } LogLevel;
void sfa_log4(_sfa_in_ const char* module, _sfa_in_ LogLevel level, _sfa_in_ const char* msgfmt, ...);
void sfa_log4(_sfa_in_ const char* module, _sfa_in_ LogLevel level, _sfa_in_ const char* file, _sfa_in_ const char* function, _sfa_in_ const int line, _sfa_in_ const char* msgfmt, ...);
#ifdef SFA_WIN
void sfa_log4(_sfa_in_ const wchar_t* module, _sfa_in_ LogLevel level, _sfa_in_ const wchar_t* msgfmt, ...);
void sfa_log4(_sfa_in_ const wchar_t* module, _sfa_in_ LogLevel level, _sfa_in_ const wchar_t* file, _sfa_in_ const wchar_t* function, _sfa_in_ const int line, _sfa_in_ const wchar_t* msgfmt, ...);
#endif

#define SFA_LOG(module, level, format, ...) sfa_log4(module, level, format,##__VA_ARGS__)
#define sfa_log_trace(module, format, ...) SFA_LOG(module, sfa::utils::Trace, format, ##__VA_ARGS__)
#define sfa_log_debug(module, format ,...) SFA_LOG(module, sfa::utils::Debug, format, ##__VA_ARGS__)
#define sfa_log_info(module, format, ...) SFA_LOG(module, sfa::utils::Info, format, ##__VA_ARGS__)
#define sfa_log_warn(module, format, ...) SFA_LOG(module, sfa::utils::Warning, format, ##__VA_ARGS__)
#define sfa_log_error(module, format, ...) SFA_LOG(module, sfa::utils::Error, format, ##__VA_ARGS__)

}
}
