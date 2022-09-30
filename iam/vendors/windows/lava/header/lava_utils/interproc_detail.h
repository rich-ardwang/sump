//
//  interproc_detail.h
//
//  Created by Colin on 2019-07-04.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once

#if defined(LAVA_WIN)
#include <windows.h>
#elif defined(LAVA_MACOSX)
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <mach/mach_time.h>
#elif defined(LAVA_LINUX)
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#else
#endif

namespace lava
{
	namespace utils
	{
#ifdef LAVA_WIN
		typedef unsigned __int64 os_highres_count_t;

		inline unsigned long get_system_tick_in_highres_counts()
		{
			typedef long(__stdcall *NtSetTimerResolution_Ptr) (unsigned long RequestedResolution, int Set, unsigned long* ActualResolution);
			HMODULE hNtdll = ::LoadLibraryA("ntdll.dll");
			NtSetTimerResolution_Ptr NtSetTimerResolution = (NtSetTimerResolution_Ptr)GetProcAddress(hNtdll, "NtSetTimerResolution");

			unsigned long curres;
			NtSetTimerResolution(10000, 0, &curres);

			__int64 freq;
			if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&freq))  // in millisecond
			{
				return (curres - 1ul) / 10000ul + 1ul;
			}
			else // in femtoseconds
			{
				__int64 count_fs = (1000000000000000LL - 1LL) / freq + 1LL;
				__int64 tick_counts = (static_cast<__int64>(curres)* 100000000LL - 1LL) / count_fs + 1LL;
				return static_cast<unsigned long>(tick_counts);
			}
		}

		inline os_highres_count_t get_current_system_highres_count()
		{
			__int64 count;
			if (!::QueryPerformanceCounter((LARGE_INTEGER*)&count))
				count = ::GetTickCount();
			return count;
		}

		inline os_highres_count_t system_highres_count_subtract(const os_highres_count_t& l, const os_highres_count_t& r)
		{
			return l - r;
		}

		inline bool system_highres_count_less_ul(const os_highres_count_t& l, unsigned long r)
		{
			return l < static_cast<os_highres_count_t>(r);
		}

		inline unsigned int get_num_cores()
		{
			SYSTEM_INFO sysinfo = {0};
			::GetSystemInfo(&sysinfo);
			return static_cast<unsigned>(sysinfo.dwNumberOfProcessors);
		}

		inline void thread_yield()
		{
			if (!::SwitchToThread())
				::Sleep(0);
		}

		inline void thread_sleep_tick()
		{
			::Sleep(1);
		}

#elif defined(LAVA_MACOSX)
		typedef unsigned long long os_highres_count_t;

		inline unsigned long get_system_tick_ns()
		{
			long ticks_per_second = ::sysconf(_SC_CLK_TCK);	// ticks per sec
			if (ticks_per_second <= 0)					// try a typical value on error
				ticks_per_second = 100;
			return 999999999ul / static_cast<unsigned long>(ticks_per_second)+1ul;
		}

		inline unsigned long get_system_tick_in_highres_counts()
		{
			mach_timebase_info_data_t info;
			mach_timebase_info(&info);
			return static_cast<unsigned long>(static_cast<double>(get_system_tick_ns()) / (static_cast<double>(info.numer) / info.denom));
		}
		inline os_highres_count_t get_current_system_highres_count()
		{
			return ::mach_absolute_time();
		}

		inline os_highres_count_t system_highres_count_subtract(const os_highres_count_t& l, const os_highres_count_t& r)
		{
			return l - r;
		}

		inline bool system_highres_count_less_ul(const os_highres_count_t &l, unsigned long r)
		{
			return l < static_cast<os_highres_count_t>(r);
		}

		inline unsigned int get_num_cores()
		{
			long cores = ::sysconf(_SC_NPROCESSORS_ONLN);
			if (cores <= 0)
				return 1;
			else if (static_cast<unsigned long>(cores) >= static_cast<unsigned long>(static_cast<unsigned int>(-1)))
				return static_cast<unsigned int>(-1);
			else
				return static_cast<unsigned int>(cores);
		}

		inline void thread_yield()
		{
			::sched_yield();
		}

		inline void thread_sleep_tick()
		{
			struct timespec rqt;
			rqt.tv_sec = 0;
			rqt.tv_nsec = get_system_tick_ns() / 2;
			::nanosleep(&rqt, 0);
		}

#elif defined(LAVA_LINUX)
		typedef struct timespec os_highres_count_t;

		inline unsigned long get_system_tick_ns()
		{
			long ticks_per_second = ::sysconf(_SC_CLK_TCK);	// ticks per sec
			if (ticks_per_second <= 0)					// try a typical value on error
				ticks_per_second = 100;
			return 999999999ul / static_cast<unsigned long>(ticks_per_second)+1ul;
		}

		inline unsigned long get_system_tick_in_highres_counts()
		{
			return get_system_tick_ns();
		}

		inline os_highres_count_t get_current_system_highres_count()
		{
			struct timespec count;
			::clock_gettime(CLOCK_MONOTONIC_RAW, &count);
			return count;
		}

		inline os_highres_count_t system_highres_count_subtract(const os_highres_count_t& l, const os_highres_count_t& r)
		{
			os_highres_count_t res;
			if (l.tv_nsec < r.tv_nsec)
			{
				res.tv_nsec = 1000000000 + l.tv_nsec - r.tv_nsec;
				res.tv_sec = l.tv_sec - 1 - r.tv_sec;
			}
			else
			{
				res.tv_nsec = l.tv_nsec - r.tv_nsec;
				res.tv_sec = l.tv_sec - r.tv_sec;
			}
			return res;
		}

		inline bool system_highres_count_less_ul(const os_highres_count_t& l, unsigned long r)
		{
			return !l.tv_sec && (static_cast<unsigned long>(l.tv_nsec) < r);
		}

		inline unsigned int get_num_cores()
		{
			long cores = ::sysconf(_SC_NPROCESSORS_ONLN);
			if (cores <= 0)
				return 1;
			else if (static_cast<unsigned long>(cores) >= static_cast<unsigned long>(static_cast<unsigned int>(-1)))
				return static_cast<unsigned int>(-1);
			else
				return static_cast<unsigned int>(cores);
		}

		inline void thread_yield()
		{
			::sched_yield();
		}

		inline void thread_sleep_tick()
		{
			struct timespec rqt;
			rqt.tv_sec = 0;
			rqt.tv_nsec = get_system_tick_ns() / 2;
			::nanosleep(&rqt, 0);
		}
#else
#endif
	}
}