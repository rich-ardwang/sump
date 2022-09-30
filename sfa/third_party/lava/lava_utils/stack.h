//
//  stack.h
//
//  Created by Colin on 2019-07-03.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include <atomic>
#include <iostream>
#include "lava_base.h"

namespace lava
{
	namespace utils
	{
		//
		// This is lock-free stack
		//
		template<typename T>
		class stack
		{
		public:
			stack() : m_head(nullptr) {}
			void push(const T& val_)
			{
				node* n = new node{ val_, m_head.load() };
				while ( !m_head.compare_exchange_strong(n->next, n) );
			}
			void push(const T&& val_)
			{
				node* n = new node{ std::move(val_), m_head.load() };
				while ( !m_head.compare_exchange_strong(n->next, n) );
			}
			bool pop(T& val_)
			{
				node* n = m_head.load();
				while ( n && !m_head.compare_exchange_strong(n, n->next) );
				if (n)
				{
					val_ = std::move(n->val);
					delete n;
					return true;
				}
				return false;
			}

		private:
			struct node
			{
				T		val;
				node*	next;
			};
			std::atomic<node*> m_head = {nullptr};
		};
	}
}

