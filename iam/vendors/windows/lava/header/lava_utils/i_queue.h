//
//  i_queue.h
//
//  Created by Colin on 2019-06-14.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once 
#include "lava_base.h"

namespace lava
{
	namespace utils
	{
		template <typename T> struct i_queue
		{
			virtual void write(__lv_in const T& value_, __lv_in bool complete_) = 0;
			virtual bool unwrite(__lv_out T* value_) = 0;
			virtual bool flush() = 0;
			virtual bool read(__lv_out T* value_) = 0;
			virtual bool probe(__lv_in bool(*fn)(__lv_in const T&)) = 0;
		};
	}
}