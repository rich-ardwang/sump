//
//  queue.h
//
//  Created by Colin on 2019-06-14.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "i_queue.h"
#include "queue_base.h"
#include "lava_sema.h"
#include "utils_traits.h"

namespace lava
{
	namespace utils
	{
		// 
		// WANING: This is a lock-free queue that supports single-thread read and
		// single-thread write. if you want multiple threads to read or write, you
		// need to synchronize these read or write threads.
		//
		template <typename T, typename Traits = default_traits> 
		class queue_t : public lava::utils::i_queue<T>, public lava::base::noncopyable
		{
			typedef typename Traits::count_type count_type;
			static const count_type ITEM_COUNT = Traits::ITEM_COUNT;

		public:
			inline queue_t(bool sync_mode = true) : m_sync_mode(sync_mode), m_sema(0)
			{
				m_queue.push();
				m_c = m_r = m_w = m_f = &m_queue.back();
			}

			//
			//  Write an item to the queue, but not flush it yet:
			//  If incomplete is set to true the item is assumed to be continued 
			//  by items subsequently written to the queue.
			//  Incomplete items are never flushed down the queue.
			//  
			inline void write(__lv_in const T& value_, __lv_in bool complete_)
			{
				//  Place the value to the queue.
				m_queue.back() = value_;
				m_queue.push();

				//  Move m_f poiter to back
				if (complete_)
					m_f = &m_queue.back();
			}

			//
			//  Pop an incomplete item from the queue:
			//
			inline bool unwrite(__lv_out T* value_)
			{
				// there is no incomplete item in queue.
				if (m_f == &m_queue.back())
					return false;

				m_queue.unpush();
				*value_ = m_queue.back();
				return true;
			}

			//
			//  Flush all the completed items into the queue
			//
			inline bool flush()
			{
				//  If there are no un-flushed items, nothing to do.
				if (m_w == m_f)
					return true;

				//  
				//  Try to set 'm_c' to 'm_f':
				//  1. CAS was unsuccessful because 'm_c' is null. which means 
				//     that the reader is asleep. 
				//  2. CAS was successful because 'm_c' is equal to m_w, which 
				//     leads to 'm_c' be set to 'm_f'
				//
				//  WARNING: the first parameter of compare_exchange_strong is
				//  lvalue(inout property). it will be changed if return false
				//
				bool ret = m_c.compare_exchange_strong(m_w, m_f, std::memory_order_seq_cst);
				if (ret)
				{
					// Just move the 'first un-flushed item' pointer to 'm_f'.
					m_w = m_f;
					return true;
				}

				// reader is asleep.
				m_c = m_f;
				m_w = m_f;

				if (m_sync_mode)
					m_sema.signal();

				return false;
			}

			//
			//  Reads an item from the queue.
			//
			inline bool read(__lv_out T* value_)
			{
				while (!has_readable())
				{
					if (false == m_sync_mode)
						return false;
					m_sema.wait();
				}

				*value_ = m_queue.front();
				m_queue.pop();
				return true;
			}

			//  Applies the function fn to the first elemenent in the queue
			inline bool probe(__lv_in bool(*fn)(__lv_in const T&))
			{
				bool rc = has_readable();
				assert(rc);

				return (*fn)(m_queue.front());
			}

		private:
			//
			//  Check whether item is available for reading.
			//
			inline bool has_readable()
			{
				//  
				//  note: during queue's lifetime m_r should never be null 
				//  unless under the condition that 'm_c' is null and call
				//  has_readable(), but it is impossible because reader is
				//  sleeping if 'm_c' is null, has_readable() has no chance
				//  to be called.
				//  there are gaps between 'front' and 'm_r', which means
				//  there are items available for reading
				//
				T* cmp = &m_queue.front();
				if (cmp != m_r)
					return true;

				// 
				// In principle, this should NOT occur because if m_c is null,
				// the reader is asleep. So add the following codes to prevent
				// caller from making mistakes.
				//
				if (nullptr == m_c.load())
					return false;

				//
				//  Run here, which means there is no prefetched value. 
				//  Set 'm_r' to 'm_c'
				//  1. CAS was unsuccessful because 'm_c' grows again, so we
				//     can prefetch some items (set 'm_r' to 'm_c')
				//  2. CAS was successful because 'front' reaches 'm_c', which
				//     means all prefetched items were used up. so set 'm_c'
				//     to null. reader will sleep.
				//
				//  WARNING: the first parameter of compare_exchange_strong is
				//  lvalue(inout property). it will be changed if return false
				//
				bool ret = m_c.compare_exchange_strong(cmp, nullptr, std::memory_order_seq_cst);
				if (false == ret)
					m_r = m_c;

				//
				//  check CAS above is successful or not.
				//  note: not use expression "m.c == nullptr" because m_c is atomic variable.
				//
				return (&m_queue.front() == m_r) ? false : true;
			}

		private:
			//
			//  the 'front' of queue points to the first prefetched item, 
			//  'front' is used only by reader thread.
			//  the 'back' of queue points to last un-flushed item. 
			//  'back' is used only by writer thread.
			//
			queue_base< T, ITEM_COUNT >	m_queue;

			//  Points to the first un-flushed item. used exclusively by writer thread.
			T*	m_w;

			//  Points to the first un-prefetched item. used exclusively by reader thread.
			T*	m_r;

			//  Points to the first incompleted item.
			T*	m_f;

			//
			//  The single point of contention between writer and reader thread.
			//  Points past the last flushed item. If it is null, reader is asleep. 
			//  This pointer should be always accessed using atomic operations.
			//
			std::atomic<T*> m_c;

			// The two varialbles below is only be used when using queue synchronously.
			bool			m_sync_mode;
			semaphore		m_sema;

			/*
				        prefetched   unprefetched      unflushed    incompleted
			-------------------------------------------------------------------------
			|   |   |$$$|$$$|$$$|+++|+++|+++|+++|???|???|???|???|###|###|###|   |   |
			-------------------------------------------------------------------------
				    front        m_r             m_w             m_f         back
				                                 m_c
				write():         put item to back, then move back to next position
				                 if this is last item of complete data, m_f-->back
				read():          check if there's readable item
				                 if true, pop front item, move front to next position
				                 if false, tell reader to sleep
				flush():         m_w-->m_f, '?' becomes '+', m_c-->m_w
				has_readable():  if front reaches m_r, m_r-->m_c, '+' becomes '$'
			*/
		};
	}
}