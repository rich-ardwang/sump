//
//  lava_sema.h
//
//  Created by Colin on 2019-06-14.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include <mutex>
#include <condition_variable>
#include "lava_base.h"

namespace lava
{
	namespace utils
	{
		class semaphore : public lava::base::noncopyable
		{
		public:
			explicit semaphore(uint32_t c) : m_count(c) {}
			~semaphore() {}

			void reset(uint32_t c = 0)
			{
				std::unique_lock<std::mutex> lock_guard(m_mutex);
				m_count = c;
			}

			void wait()
			{
				std::unique_lock<std::mutex> lock_guard(m_mutex);
				--m_count;
				m_cond.wait(lock_guard, [this]()->bool{ return this->m_count >= 0; });
			}

			void signal()
			{
				std::lock_guard<std::mutex> lock_guard(m_mutex);
				if (++m_count < 1)
					m_cond.notify_one();
			}

			semaphore(const semaphore&) = delete;
			semaphore& operator = (const semaphore&) = delete;

		private:
			std::mutex				m_mutex;
			std::condition_variable m_cond;
			volatile int32_t		m_count;
		};
	}
}