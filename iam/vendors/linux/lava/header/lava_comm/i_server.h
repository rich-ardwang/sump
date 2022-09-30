//
//  i_sessions.h
//
//  Created by Colin on 2019-08-13.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "lava_base.h"
#include "i_server_listener.h"
#include "i_server_helper.h"

namespace lava
{
	namespace comm
	{
		struct i_server
		{
			virtual void init(__lv_in uint32_t worker_threads) = 0;
			virtual bool start_server(__lv_in i_server_listener* listener, __lv_in const char* host, __lv_in const char* port) = 0;
			virtual bool start_server(__lv_in i_server_listener* listener, __lv_in const char* host, __lv_in const char* port, __lv_in const char* cluster_server_info, __lv_in const char* server_name, __lv_in lava_service_type type) = 0;
			virtual void stop_server() = 0;
			virtual void reply_message(__lv_in proto::i_message* msg) = 0;
			virtual void publish_message(__lv_in proto::i_message* msg, __lv_in const char* topic) = 0;
		};
	}
}