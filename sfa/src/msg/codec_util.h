#pragma once
#include <sfa/msg/value_field.h>
#include "codec_type.h"

namespace sfa {
namespace msg {

class MessageCodecUtil{
public:
	static Byte GetFieldTypeValue(Byte fieldInfo);
	static Byte GetFieldTypeDescValue(Byte fieldInfo);

	static Byte GenFieldInfo(FieldType type, FieldDesc desc);
	static Byte GenFieldInfo(Byte type, FieldDesc desc);
	static Byte GenFieldInfo(Byte type, Byte desc);

	static ByteArray EncodeFieldValue(FieldType type, const IFieldValue* value);
	static IFieldValue* DecodeFieldValue(FieldType type, ByteArray& buffer);

	static IFieldValue* GetValue(FieldType type, FieldDesc desc, ByteArray& input);

	static Byte GetFieldType(FieldType type);
	static FieldType GetFieldType(Byte bFieldType);

	static Byte GetFieldDesc(FieldDesc desc);
	static FieldDesc GetFieldDesc(Byte fieldDesc);

	static int64_t DateToInt(time_t t);
	static time_t IntToDate(int64_t val);
	static int64_t TimeToInt(time_t t);
	static time_t IntToTime(int64_t val);

private:
	static TypeCodec* GetTypeCodec(FieldType type);
};

}
}