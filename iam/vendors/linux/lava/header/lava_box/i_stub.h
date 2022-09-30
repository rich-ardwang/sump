//
//  i_stub.h
//
//  Created by Colin on 2019-08-19.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "i_message_listener.h"
#include "i_event_listener.h"

namespace lava
{
	namespace box
	{
		struct i_stub : public base::i_object
		{
			virtual void reply_message(__lv_in lava::proto::i_message* msg) = 0;
			virtual void publish_message(__lv_in lava::proto::i_message* msg, __lv_in const char* topic) = 0;
		};
	}
}