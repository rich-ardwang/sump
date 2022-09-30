#pragma once
#include <sfa/msg/msg_body.h>
#include <sfa/msg/value_array.h>
#include <sfa/msg/value_kv_array.h>

namespace sfa {
namespace msg {

class MessageBodyFullCodec{
public:
	class FieldValueTypeDesc{
	public:
		FieldValueTypeDesc(FieldType type, FieldDesc desc){
			type_ = type;
			desc_ = desc;
		}

	public:
		FieldType type_;
		FieldDesc desc_;
	};

	enum ArrayFlag{
		NORMAL, NEW_ROW, END
	};

public:
	static ByteArray Encode(const IMessageBody* body);
	static bool Decode(ByteArray& buffer, IMessageBody* body);

	static ByteArray GetFieldInfoByte(Byte fieldInfo);
	static FieldValueTypeDesc GetFieldInfo(Byte fieldInfo);

private:
	static ByteArray EncodeField(int fid, IFieldValue* value);

	static ArrayFlag GetArrayFlag(Byte fieldInfo);
	static ByteArray EncodeArray(ArrayValue* arrValue);	
	static ArrayValue* DecodeArray(ByteArray& buffer);

	static ByteArray EncodeKVArray(const KVArrayValue* kvArrValue);
	static KVArrayValue* DecodeKVArray(ByteArray& buffer);
};

}
}