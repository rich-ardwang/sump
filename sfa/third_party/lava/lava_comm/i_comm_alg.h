//
//  i_comm_alg.h
//
//  Created by Colin on 2019-07-01.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once
#include "lava_base.h"

namespace lava
{
	namespace comm
	{
		struct i_zip_alg
		{
			virtual bool zip(__lv_in uint8_t* in_buf, __lv_in uint32_t in_size, __lv_out uint8_t*& out_buf, __lv_out uint32_t& out_size) = 0;
			virtual bool unzip(__lv_in uint8_t* in_buf, __lv_in uint32_t in_size, __lv_out uint8_t*& out_buf, __lv_out uint32_t& out_size) = 0;
			virtual void free_buffer(__lv_in uint8_t* buf) = 0;
		};

		struct i_enc_alg
		{
			virtual bool encrypt(__lv_in uint8_t* in_buf, __lv_in uint32_t in_size, __lv_out uint8_t*& out_buf, __lv_out uint32_t& out_size) = 0;
			virtual bool decrypt(__lv_in uint8_t* in_buf, __lv_in uint32_t in_size, __lv_out uint8_t*& out_buf, __lv_out uint32_t& out_size) = 0;
			virtual void free_buffer(__lv_in uint8_t* buf) = 0;
		};
	}
}