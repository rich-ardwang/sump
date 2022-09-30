//
//  i_connector.h
//
//  Created by Colin on 2019-07-25.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "lava_base.h"
#include "comm_protocol.h"

namespace lava
{
	namespace comm
	{
		struct i_client_session;
		struct i_connector
		{
			virtual void init(__lv_in uint32_t io_threads) = 0;
			virtual i_client_session* connect(__lv_in const char* server_name, __lv_in comm_protocol proto_, __lv_in const client_login_info* info_, __lv_in bool use_shared_session) = 0;
			virtual void clear() = 0;
		};
	}
}