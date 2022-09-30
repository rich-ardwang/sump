//
//  i_client_session.h
//
//  Created by Colin on 2019-06-29.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "i_message_listener.h"
#include "i_event_listener.h"

namespace lava
{
	namespace comm
	{
		struct i_client_session : public base::i_object
		{
			virtual void set_auto_reconnect(__lv_in bool is_auto, __lv_in uint32_t try_times) = 0;
			virtual void manual_reconnect() = 0;
			virtual void set_sync_timeout(__lv_in int32_t seconds) = 0;
			virtual void set_signal_id(__lv_in const char* signal_id) = 0;
			virtual void advise(__lv_in i_message_listener* sinker, __lv_out uint32_t& cookie) = 0;
			virtual void unadvise(__lv_in uint32_t& cookie) = 0;
			virtual bool send_request(__lv_in proto::i_message* req_msg, __lv_out proto::i_message*& reply_msg) = 0; // use default timeout
			virtual bool send_request(__lv_in proto::i_message* req_msg, __lv_in int32_t timeout_seconds, __lv_out proto::i_message*& reply_msg) = 0;
			virtual uint32_t post_request(__lv_in proto::i_message* req_msg, __lv_in int32_t cookie) = 0;
			virtual void subscribe_with_filter(__lv_in char* topics, __lv_in proto::i_message* msg, __lv_in i_message_listener* listener) = 0;
			virtual void unsubscribe_with_filter(__lv_in char* topics, __lv_in proto::i_message* msg, __lv_in i_message_listener* listener) = 0;
			virtual void subscribe(__lv_in char** topics, __lv_in int32_t count, __lv_in i_message_listener* listener) = 0;
			virtual void subscribe(__lv_in char** topics, __lv_in int32_t count, __lv_in const char* svr_id, __lv_in i_message_listener* listener) = 0;
			virtual void unsubscribe(__lv_in char** topics, __lv_in int32_t count, __lv_in i_message_listener* listener) = 0;
			virtual void add_event_listener(__lv_in i_event_listener* listener) = 0;
			virtual void remove_event_listener(__lv_in i_event_listener* listener) = 0;
		};
	}
}