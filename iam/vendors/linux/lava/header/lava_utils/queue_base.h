//
//  queue_base.h
//
//  Created by Colin on 2019-06-14.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include <atomic>
#include "lava_base.h" 
#include <assert.h>

namespace lava
{
	namespace utils
	{
		template <typename T, int N> class queue_base : public lava::base::noncopyable
		{
		public:
			inline queue_base() : m_spare_block(0)
			{
				m_begin_block = allocate_block();
				assert(m_begin_block);
				m_begin_pos = 0;
				m_back_block = nullptr;
				m_back_pos = 0;
				m_end_block = m_begin_block;
				m_end_pos = 0;
			}

			inline ~queue_base()
			{
				while (true) 
				{
					if ( m_begin_block == m_end_block ) 
					{
						free(m_begin_block);
						break;
					}
					block_t* o = m_begin_block;
					m_begin_block = m_begin_block->next;
					free(o);
				}

				block_t* sc = m_spare_block.exchange(nullptr, std::memory_order_acq_rel);
				free(sc);
			}

			// 
			//  Return reference to the front element of the queue.
			//  If the queue is empty, behaviour is undefined.
			//
			inline T& front() { return m_begin_block->values[m_begin_pos]; }

			//
			//  Return reference to the back element of the queue.
			//  If the queue is empty, behaviour is undefined.
			//
			inline T& back() { return m_back_block->values[m_back_pos]; }

			//
			//  Adds an element to the back end of the queue.
			//
			inline void push()
			{
				m_back_block = m_end_block;
				m_back_pos = m_end_pos;
				if ( ++m_end_pos != N )
					return;

				block_t* sb = m_spare_block.exchange(nullptr, std::memory_order_acq_rel);
				if ( sb ) 
				{
					m_end_block->next = sb;
					sb->prev = m_end_block;
				}
				else 
				{
					m_end_block->next = allocate_block();
					assert(m_end_block->next);
					m_end_block->next->prev = m_end_block;
				}
				m_end_block = m_end_block->next;
				m_end_pos = 0;
			}

			//
			//  Removes element from the back side of the queue.
			//  Take care: Caller is responsible for destroying the object unpushed.
			//  
			//  The caller must also guarantee that the queue isn't empty when
			//  unpush is called. It cannot be done automatically as the read
			//  side of the queue can be managed by different, completely
			//  unsynchronised thread.
			//
			inline void unpush()
			{
				//  First, move 'back' one position backwards.
				if ( m_back_pos )
					--m_back_pos;
				else 
				{
					m_back_pos = N - 1;
					m_back_block = m_back_block->prev;
				}

				//  Now, move 'end' position backwards. Note that obsolete end block
				//  is not used as a spare block. The analysis shows that doing so
				//  would require free and atomic operation per block deallocated
				//  instead of a simple free.
				if ( m_end_pos )
					--m_end_pos;
				else
				{
					m_end_pos = N - 1;
					m_end_block = m_end_block->prev;
					free(m_end_block->next);
					m_end_block->next = nullptr;
				}
			}

			//
			//  Removes an element from the front side of the queue.
			//
			inline void pop()
			{
				if ( ++m_begin_pos == N )
				{
					block_t* o = m_begin_block;
					m_begin_block = m_begin_block->next;
					m_begin_block->prev = nullptr;
					m_begin_pos = 0;

					//
					//  'o' has been more recently used than m_spare_block,
					//  so use 'o' as the spare.
					//
					block_t* sb = m_spare_block.exchange(o, std::memory_order_acq_rel);
					free(sb);
				}
			}

		private:
			struct block_t
			{
				T			values[N]; // hold N elements.
				block_t*	prev;
				block_t*	next;
			};

			inline block_t* allocate_block()
			{
				return (block_t*)malloc(sizeof(block_t));
			}

		private:
			block_t*	m_begin_block;
			int			m_begin_pos;

			//  like the end iterator to container: be on call
			block_t*	m_back_block;
			int			m_back_pos;

			//  like the guard.
			block_t*	m_end_block;
			int			m_end_pos;

			//
			//  hold the most recently freed block as later use
			//  in order to save time from calling malloc/free.
			//
			std::atomic<block_t*> m_spare_block;
		};
	}
}
