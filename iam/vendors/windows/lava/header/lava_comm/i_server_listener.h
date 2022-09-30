//
//  i_server_listener.h
//
//  Created by Colin on 2019-08-13.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once 
#include "lava_base.h"
#include "i_message.h"

namespace lava
{
	namespace comm
	{
		struct i_server_listener : public base::i_object
		{
			enum auth_result: uint8_t { identity_ok = 1, username_error, password_error };
			virtual auth_result on_authentication(__lv_in const char* username, __lv_in const char* password) = 0;
			virtual void on_request(__lv_in proto::i_message* msg) = 0;
			virtual void on_subscribe(__lv_in const char* topic) = 0;
			virtual void on_unsubscribe(__lv_in const char* topic) = 0;
		};
	}
}
