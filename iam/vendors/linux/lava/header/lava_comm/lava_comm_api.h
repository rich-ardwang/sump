//
//  lava_comm_api.h
//
//  Created by Colin on 2019-07-25.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once


#ifdef LAVA_WIN
#ifdef LAVA_COMM_EXPORTS
#define lava_comm_api __declspec(dllexport)
#else
#define lava_comm_api __declspec(dllimport)
#endif
#elif defined (LAVA_MACOSX)
#define lava_comm_api __attribute__ ((visibility ("default")))
#else // LAVA_LINUX
#define lava_comm_api 
#endif

#include "i_connector.h"
#include "i_server.h"

extern "C"
{
	lava_comm_api lava::comm::i_connector* get_connector();
	lava_comm_api void free_connector();
	lava_comm_api lava::comm::i_server* get_server();
	lava_comm_api void free_server();
};