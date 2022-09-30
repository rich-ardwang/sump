//
//  lava_proto_api.h
//
//  Created by Colin on 2019-10-15.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#pragma once


#ifdef LAVA_WIN
#ifdef LAVA_PROTO_EXPORTS
#define lava_proto_api __declspec(dllexport)
#else
#define lava_proto_api __declspec(dllimport)
#endif
#elif defined (LAVA_MACOSX)
#define lava_proto_api __attribute__ ((visibility ("default")))
#else // LAVA_LINUX
#define lava_proto_api 
#endif

#include "i_group_message_handler.h"

extern "C"
{
	lava_proto_api lava::proto::i_group_message_handler* create_group_message_handler();
	lava_proto_api int parse_messages(__lv_in char* input_buffer, __lv_in uint32_t input_len, __lv_out char*& mask, __lv_out lava::proto::i_message*& msgs, __lv_out int& msg_num, __lv_in int client_model);
	lava_proto_api int to_message(__lv_inout lava::proto::i_message* msg, __lv_inout char* buff);
	lava_proto_api int to_bytes(__lv_out char* output_buffer, __lv_in lava::proto::i_message* msg, __lv_out char* data, __lv_out uint16_t& data_len);
	lava_proto_api int try_rescue(__lv_in uint8_t* input_buffer, __lv_in uint16_t input_len);
	lava_proto_api int decompse_group_message(__lv_out lava::proto::i_message**& msgs, __lv_out uint32_t& count, __lv_out char* buffer, __lv_inout uint32_t& buffer_len, __lv_in lava::proto::i_message* msg);
	lava_proto_api bool parse_user_info(__lv_in lava::proto::i_message* ctl_msg, __lv_out char* name, __lv_in int name_len,
										__lv_out char* psw, __lv_in int psw_len, __lv_out char* client_id, __lv_in int id_len);
	lava_proto_api bool parse_login_result(__lv_in lava::proto::i_message* ctl_msg, __lv_in uint8_t& code, __lv_in uint32_t& conn_id);
	lava_proto_api lava::proto::i_message* get_login_message(__lv_in const char* name, __lv_in const char* password);
	lava_proto_api lava::proto::i_message* get_vs_login(__lv_in lava::proto::i_message* msg);
	lava_proto_api lava::proto::i_message* get_logout_message();
	lava_proto_api lava::proto::i_message* get_login_response(__lv_in uint8_t ctrl_code, __lv_in int result, __lv_in char* error, __lv_in uint32_t conn_id);
	lava_proto_api lava::proto::i_message* get_destination(__lv_in char* topic, __lv_in uint8_t ctrl_code);
	lava_proto_api lava::proto::i_message* get_message(uint8_t type);
	lava_proto_api bool parse_topics(__lv_in lava::proto::i_message* ctl_msg, __lv_out char**& topics, __lv_out uint32_t& count);
	lava_proto_api void free_topics(char** pointer, uint32_t count);
	lava_proto_api void free_memory(void* pointer, bool is_array = false);

};