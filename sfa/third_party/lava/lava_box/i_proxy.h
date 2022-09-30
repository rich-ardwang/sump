//
//  i_proxy.h
//
//  Created by Colin on 2019-07-24.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "i_message_listener.h"
#include "i_event_listener.h"

namespace lava
{
	namespace box
	{
		struct i_proxy : public base::i_object
		{
			virtual void set_auto_reconnect(__lv_in bool is_auto, __lv_in uint32_t try_times) = 0;
			virtual void manual_reconnect() = 0;
			virtual void set_sync_timeout(__lv_in int32_t seconds) = 0;
			virtual void advise(__lv_in comm::i_message_listener* sinker, __lv_out uint32_t& cookie) = 0;
			virtual void unadvise(__lv_in uint32_t& cookie) = 0;
			virtual bool request(comm::i_message* req_msg, __lv_out comm::i_message*& reply_msg) = 0; // use default timeout;
			virtual bool request(__lv_in comm::i_message* req_msg, __lv_out comm::i_message*& reply_msg, __lv_in int32_t timeout_seconds) = 0;
			virtual uint32_t request_asyn(__lv_in comm::i_message* req_msg, __lv_in int32_t cookie) = 0;
			virtual void subscribe_with_filter(__lv_in char* topics, __lv_in comm::i_message* msg, __lv_in comm::i_message_listener* listener) = 0;
			virtual void unsubscribe_with_filter(__lv_in char* topics, __lv_in comm::i_message* msg, __lv_in comm::i_message_listener* listener) = 0;
			virtual void subscribe(__lv_in char** topics, __lv_in int32_t count, __lv_in comm::i_message_listener* listener) = 0;
			virtual void unsubscribe(__lv_in char** topics, __lv_in int32_t count, __lv_in comm::i_message_listener* listener) = 0;
			virtual void interest_event(__lv_in comm::i_event_listener* listener) = 0;
			virtual void cancel_event(__lv_in comm::i_event_listener* listener) = 0;
		};
	}
}