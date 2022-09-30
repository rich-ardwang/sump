//
//  lava_crt.h
//
//  Created by Colin on 2019-07-15.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "lava_base.h"

namespace lava
{
	namespace utils
	{
		class fake_mutex : public base::noncopyable
		{
		public:
			inline void lock() {}
			inline void lock() const {}
			inline bool try_lock() { return true; }
			inline bool try_lock() const { return true; }
			inline void unlock() {}
			inline void unlock() const {}
		};
	}
}