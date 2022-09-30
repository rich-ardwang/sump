#pragma once
#include "i_message_body.h"
#include "message_const.h"


namespace lava
{
	namespace comm
	{
		class i_message
		{
		public:
			virtual ~i_message() {}
			virtual void reference() = 0;
			virtual void dereference() = 0;

			virtual i_message_body* get_message_body() = 0;

			virtual bool set_msg_type(uint8_t type) = 0;
			virtual uint8_t get_msg_type() = 0;

			virtual void link_message(i_message* link_msg) = 0;

			virtual i_message* link_message() = 0;

			virtual int get_msg_size() = 0;
			virtual int get_web_msg_size() = 0;

			virtual void set_stream(char* data, int size) = 0;
			virtual void get_stream(char*& data, int& size) = 0;

			virtual void set_web_stream(char* data, int size) = 0;
			virtual void get_web_stream(char*& data, int& size) = 0;



			// control message
			virtual void set_control_code(uint8_t code) = 0;
			virtual uint8_t get_control_code() = 0;

			// data message
			virtual i_message* create_reply_message() = 0;
			virtual void set_destination(const char* desc, int len) = 0;
			virtual const char* get_destination() = 0;
			virtual void set_msg_id(uint32_t msg_id) = 0;
			virtual uint32_t get_msg_id() = 0;
			virtual void set_update_type(uint8_t update_type) = 0;
			virtual uint8_t get_update_type() = 0;

			virtual void set_reply_to(const char* reply_to, int len) = 0;
			virtual const char* get_reply_to() = 0;

			// 设置和获取服务唯一标识
			virtual void set_svr_id(const char* svr_id, int len) = 0;
			virtual const char* get_svr_id() = 0;

			// instead of svr_id if message type MSG_TYPE_CODE_REG
			virtual void set_func_code(uint16_t func_code) = 0;
			virtual uint16_t get_func_code() = 0;

			// 设置和获取服务唯一标识
			virtual void set_signal_id(const char* signal, int len) = 0;
			virtual const char* get_signal_id() = 0;

			//设置和获取Client连接ID
			virtual void set_conn_id(uint32_t conn_id) = 0;
			virtual uint32_t get_conn_id() = 0;
		};
	}
}