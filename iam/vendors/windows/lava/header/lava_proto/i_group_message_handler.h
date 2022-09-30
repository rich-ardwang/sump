//
//  group_message_handler.h
//
//  Re-edited by Colin on 2019-07-08.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "lava_base.h"
#include "i_message.h"

namespace lava
{
	namespace proto
	{
		struct i_group_message_handler
		{
			virtual ~i_group_message_handler() {};
			virtual int add_message(__lv_in i_message* data_msg, __lv_out i_message*& out_msg) = 0;
		};
	}
}