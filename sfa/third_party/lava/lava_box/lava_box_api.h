//
//  lava_box_api.h
//
//  Created by Colin on 2019-07-25.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once


#ifdef LAVA_WIN
#ifdef LAVA_BOX_EXPORTS
#define lava_box_api __declspec(dllexport)
#else
#define lava_box_api __declspec(dllimport)
#endif
#elif defined (LAVA_MACOSX)
#define lava_box_api __attribute__ ((visibility ("default")))
#else // LAVA_LINUX
#define lava_box_api 
#endif

#include "i_proxy.h"
#include "i_stub.h"
#include "i_server_helper.h"
#include "i_server_listener.h"
#include "comm_protocol.h"

extern "C"
{
	lava_box_api void init_environment( __lv_in uint32_t client_io_threads = 1,						/*use lava_box as client side*/
										__lv_in uint32_t server_work_threads = 8);					/*use lava_box as server side*/
	lava_box_api void clear_environment();
}
lava_box_api lava::base::i_auto_ptr<lava::box::i_proxy> create_proxy(__lv_in const char* server_name_, __lv_in lava::comm::client_login_info* info_, __lv_in bool use_shared_session = false);
lava_box_api lava::base::i_auto_ptr<lava::box::i_proxy> create_proxy(__lv_in const char* server_name_, __lv_in const char* config_file_);
lava_box_api lava::base::i_auto_ptr<lava::box::i_stub> create_stub(__lv_in const char* server_name_, __lv_in lava::comm::i_server_listener* listener, __lv_in const char* host_, __lv_in const char* port_);
lava_box_api lava::base::i_auto_ptr<lava::box::i_stub> create_stub(__lv_in const char* config_file_, __lv_in lava::comm::i_server_listener* listener);
lava_box_api lava::base::i_auto_ptr<lava::box::i_stub> create_stub(__lv_in const char* server_name_, __lv_in lava::comm::i_server_listener* listener, __lv_in const char* host_, __lv_in const char* port_, __lv_in const char* cluster_server_info, __lv_in const char* server_name, __lv_in lava::comm::lava_service_type server_type);