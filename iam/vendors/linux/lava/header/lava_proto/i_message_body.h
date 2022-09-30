#pragma once
#include "lava_base.h"

namespace lava
{
	namespace proto
	{
		class i_message_body
		{
		public:
			struct field
			{
				field() : field_type(0), position(0), len(0)
				{
					_val.i64Val = 0;
				}
				uint8_t		field_type;
				uint16_t	position;
				uint16_t	len;

				union {
					double		dbVal;
					int64_t		i64Val;
					char*		sVal;
					float		fVal;
					uint32_t	uiVal;
					int16_t		iVal;
				}_val;
			};

			enum : uint8_t { NONE = 0, INT = 1, LONG = 2, SHORT = 3, FLOAT = 4, DOUBLE = 5, STRING = 6, BYTES = 7 };
			virtual uint16_t add_string(__lv_in uint16_t pos, __lv_in const char* value) = 0;
			virtual uint16_t add_int(__lv_in uint16_t pos, __lv_in uint32_t value) = 0;
			virtual uint16_t add_long(__lv_in uint16_t pos, __lv_in int64_t value) = 0;
			virtual uint16_t add_short(__lv_in uint16_t pos, __lv_in int16_t value) = 0;
			virtual uint16_t add_float(__lv_in uint16_t pos, __lv_in float value) = 0;
			virtual uint16_t add_double(__lv_in uint16_t pos, __lv_in double value) = 0;
			virtual uint16_t add_bytes(__lv_in uint16_t pos, __lv_in const char* value, __lv_in uint16_t len) = 0;
			virtual char* get_string(__lv_in uint16_t pos) = 0;
			virtual uint32_t get_int(__lv_in uint16_t pos) = 0;
			virtual int64_t get_long(__lv_in uint16_t pos) = 0;
			virtual int16_t get_short(__lv_in uint16_t pos) = 0;
			virtual float get_float(__lv_in uint16_t pos) = 0;
			virtual double get_double(__lv_in uint16_t pos) = 0;
			virtual uint16_t get_bytes(__lv_in uint16_t pos, __lv_out char*& value) = 0;
			//virtual int get_gzip_string(__lv_in uint16_t pos, __lv_inout char* value, __lv_inout int& value_len) = 0;
			//virtual uint16_t add_gzip_string(__lv_in uint16_t pos, __lv_in const char* src, __lv_in int& src_len, __lv_out char*& des, __lv_out size_t& des_len) = 0;

			virtual void add_field(__lv_in uint16_t pos, __lv_in field* pField) = 0;
			virtual void remove_field(__lv_in uint16_t pos) = 0;
			virtual void set_blob_field(__lv_in uint16_t pos, __lv_in char* bytes, __lv_in uint32_t len) = 0;
			virtual uint16_t get_blob_field(__lv_out char*& bytes, __lv_out uint32_t& len) = 0;
			virtual uint8_t has_field(__lv_in uint16_t pos) = 0;
			virtual uint16_t get_fields(__lv_out field** list) = 0;
			~i_message_body() {}
			virtual void clear() = 0;

			//virtual  void set_stream(__lv_in char* data, __lv_in int size) = 0;
			//virtual  void get_stream(__lv_out char*& data, __lv_out int& size) = 0;

			virtual uint16_t get_size() = 0;
		};
	}
}