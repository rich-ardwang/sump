//
//  spin_lock.h
//
//  Created by Colin on 2019-07-03.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include <atomic>
#include "lava_base.h"

namespace lava
{
	namespace utils
	{
		class spin_lock : public lava::base::noncopyable
		{
		public:
			spin_lock()
			{
				m_locked.clear(std::memory_order_release);
			}
			inline bool try_lock()
			{
				return false == m_locked.test_and_set(std::memory_order_acquire);
			}
			inline void lock() 
			{
				while ( m_locked.test_and_set(std::memory_order_acquire) );
			}
			inline void unlock() 
			{
				m_locked.clear(std::memory_order_release);
			}

		private:
			#ifdef LAVA_WIN
			std::atomic_flag m_locked /*= ATOMIC_FLAG_INIT*/; // initialization not supported in VS2013
			#else // LAVA_MACOSX or LAVA_LINUX
			std::atomic_flag m_locked = ATOMIC_FLAG_INIT;
			#endif
		};
	}
}