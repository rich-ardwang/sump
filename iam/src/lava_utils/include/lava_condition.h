//
//  lava_condition.h
//
//  Created by Colin on 2019-08-20.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "lava_base.h"

namespace lava
{
	namespace utils
	{
		class condition : public base::noncopyable
		{
		public:
			condition() : m_pred(false) {};

			inline void reset()
			{
				m_pred = false;
			}

			void wait()
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cond.wait(lock, [this]() { return m_pred; });
			}

			template <class Rep, class Period>
			bool wait_for(const std::chrono::duration<Rep, Period>& rel_time)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				return m_cond.wait_for(lock, rel_time, [this]() { return m_pred; });
			}

			void notify_one()
			{
				std::unique_lock<std::mutex> lock_guard(m_mutex);
				m_pred = true;
				m_cond.notify_one();
			}

			void notify_all()
			{
				std::unique_lock<std::mutex> lock_guard(m_mutex);
				m_pred = true;
				m_cond.notify_all();
			}

		private:
			volatile bool			m_pred;
			std::mutex				m_mutex;
			std::condition_variable	m_cond;
		};
	}
}