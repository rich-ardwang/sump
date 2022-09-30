/***************************************************************************************************
*Copyright(C): 2019, Sumscope
*FileName    : atomic.h
*Author      : lei.wang@sumscope.com
*Version     : 1.0
*Date        : 2019-10-11
*Desc        : atomic operation
*History     : Add by Lei.Wang 2019-10-11
***************************************************************************************************/
#ifndef __LAVA_ATOMIC_H__
#define __LAVA_ATOMIC_H__

#ifndef LAVA_WIN
#ifndef __linux__
#error "Unknown os platform, _WIN32 or __linux__ not defined."
#endif
#endif

namespace lava
{

	int inline atomic_and_fetch(int* addend, int value);
	bool inline atomic_fetch_and_bit_set(int* addend, int bit);
	bool inline atomic_fetch_and_bit_reset(int* addend, int bit);
	int inline atomic_fetch_and_add(int* addend, int value);
	int inline atomic_fetch_and_sub(int* addend, int value);
	int inline atomic_fetch_and_set(int *target, int value);
	int inline atomic_increment(int* addend);
	int inline atomic_decrement(int* addend);
	bool inline atomic_cmp_and_swap(int* targe, int value, int cmp);

}

#ifdef LAVA_WIN
#include <intrin.h>
#pragma intrinsic(_InterlockedAnd)
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_interlockedbittestandset)
#pragma intrinsic(_interlockedbittestandreset)
#endif

namespace lava
{

	int inline atomic_and_fetch(int* addend, int value)
	{
#ifdef LAVA_WIN
		return _InterlockedAnd((long*)addend, value);
#else
		return __sync_and_and_fetch(addend, value);
#endif
	}

	bool inline atomic_fetch_and_bit_set(int* addend, int bit)
	{
#ifdef LAVA_WIN
		return !(_interlockedbittestandset((long*)addend, bit));
#else
		return !(__sync_fetch_and_or(addend, 1<<bit) & (1<<bit));
#endif
	}

	bool inline atomic_fetch_and_bit_reset(int* addend, int bit)
	{
#ifdef LAVA_WIN
		return 0 != (_interlockedbittestandreset((long*)addend, bit));
#else
		return 0 != (__sync_fetch_and_and(addend, ~(1<<bit)) & (1<<bit));
#endif
	}

	int inline atomic_fetch_and_add(int* addend, int value)
	{
#ifdef LAVA_WIN
		return (int)_InterlockedExchangeAdd((long*)addend, value);
#else
		return __sync_fetch_and_add(addend, value);
#endif
	}

	int inline atomic_fetch_and_sub(int* addend, int value)
	{
#ifdef LAVA_WIN
		return (int)_InterlockedExchangeAdd((long*)addend, ~value + 1);
#else
		return __sync_fetch_and_sub(addend, value);
#endif
	}

	int inline atomic_fetch_and_set(int *target, int value)
	{
#ifdef LAVA_WIN
		return (int)_InterlockedExchange((long*)target, value);
#else
		return __sync_lock_test_and_set(target, value);
#endif
	}

	int inline atomic_increment(int* addend)
	{
#ifdef LAVA_WIN
		return (long)_InterlockedIncrement((long*)addend);
#else
		return __sync_add_and_fetch(addend, 1);
#endif
	}

	int inline atomic_decrement(int* addend)
	{
#ifdef LAVA_WIN
		return (long)_InterlockedDecrement((long*)addend);
#else
		return __sync_sub_and_fetch(addend, 1);
#endif
	}

	bool inline atomic_cmp_and_swap(int* targe, int oldval, int newval)
	{
#ifdef LAVA_WIN
		return oldval == _InterlockedCompareExchange((long*)targe, newval, oldval);
#else
		return __sync_bool_compare_and_swap(targe, oldval, newval);
#endif
	}

}

#endif	/* __LAVA_ATOMIC_H__ */
