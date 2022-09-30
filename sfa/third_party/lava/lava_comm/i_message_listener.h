//
//  i_message_listener.h
//
//  Created by Colin on 2019-07-24.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "lava_base.h"

namespace lava
{
	namespace comm
	{
		class i_message;
		struct i_message_listener : public base::i_object
		{
			virtual void on_message(__lv_in i_message* msg) = 0;
			virtual void on_reply(__lv_in i_message* msg, __lv_in const char* topic) = 0;
		};
	}
}