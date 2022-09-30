//
//  i_connection.h
//
//  Created by Colin on 2019-06-28.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//

#pragma once
#include "lava_base.h"

namespace lava
{
	namespace comm
	{
		struct i_data_listener
		{
			virtual void on_data_recv(__lv_in const uint8_t* buffer, __lv_in uint32_t size) = 0;
		};

		struct i_connection : public base::i_object
		{
			virtual void send_data(__lv_in const uint8_t* buffer, __lv_in uint32_t size) = 0;
			virtual void recv_data(__lv_in i_data_listener* listener) = 0;
			virtual void close() = 0; // for active close.
		};
	}
}

