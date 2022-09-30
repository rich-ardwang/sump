//
//  spin_wait.h
//
//  Created by Colin on 2019-07-04.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "interproc_detail.h"
#include "lava_base.h"

namespace lava
{
	namespace utils
	{
		template<int dummy = 0>
		class num_core_holder
		{
		public:
			static unsigned int get()
			{
				return num_cores ? num_cores : get_num_cores();
			}

		private:
			static unsigned int num_cores;
		};

		template<int dummy>
		unsigned int num_core_holder<dummy>::num_cores = get_num_cores();

		class spin_wait
		{
		public:
			static const unsigned int nop_pause_limit = 32u;
			spin_wait() : m_count_start(), m_ul_yield_only_counts(), m_k() {}

			void yield()
			{
				if (!m_k)							// lazy initialization of limits
				{
					this->init_limits();
				}

				if (m_k < (nop_pause_limit >> 2))	// nop tries
				{

				}
				#if defined(LAVA_SMT_PAUSE)
				else if (m_k < nop_pause_limit)
				{
					smt_pause();
				}
				#endif
				else								// yield/sleep strategy
				{
					if (m_k == nop_pause_limit)		// lazy initialization of tick information
					{
						this->init_tick_info();
					}
					else if (this->yield_or_sleep())
					{
						thread_yield();
					}
					else
					{
						thread_sleep_tick();
					}
				}
				++m_k;
			}

		private:
			void init_limits()
			{
				unsigned int num_cores = num_core_holder<0>::get();
				m_k = num_cores > 1u ? 0u : nop_pause_limit;
			}

			void init_tick_info()
			{
				m_ul_yield_only_counts = get_system_tick_in_highres_counts();
				m_count_start = get_current_system_highres_count();
			}

			// returns: true means yield must be called; false means sleep must be called
			bool yield_or_sleep()
			{
				if (!m_ul_yield_only_counts)	// if yield-only limit was reached then yield one in every two tries
				{
					return (m_k & 1u) != 0;
				}
				else							// try to see if we've reached yield-only time limit
				{
					const os_highres_count_t now = get_current_system_highres_count();
					const os_highres_count_t elapsed = system_highres_count_subtract(now, m_count_start);
					if (!system_highres_count_less_ul(elapsed, m_ul_yield_only_counts))
					{
						m_ul_yield_only_counts = 0ul;	// yield-only time reached, now it's time to sleep
						return false;
					}
				}
				return true;   // otherwise yield
			}

			os_highres_count_t	m_count_start;
			unsigned long		m_ul_yield_only_counts;
			unsigned int		m_k;
		};
	}
}