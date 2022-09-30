//
//  i_event_listener.h
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
		struct i_event_listener : public base::i_object
		{
			enum
			{
				event_conn_begin,
				event_conn_success,
				event_conn_disconn,
				event_conn_closed,

				event_algo_nego_begin,
				event_algo_nego_success,

				event_login_begin,
				event_login_no_response,
				event_login_success,

				event_logout_begin,
				event_logout_success,

				event_lowest_water_mark,
				event_highest_water_mark,

				exception_conn_failed = 100,
				exception_algo_nego_failed,
				exception_login_failed,
			};
			virtual void on_event(int32_t event_code, void* event_data) = 0;
			virtual void on_exception(int32_t error_code, const char* what) = 0;
		};
	}
}
