#pragma once
#include <sfa/msg/msg_body.h>
#include <sfa/msg/msg_context.h>
#include <sfa/msg/value_kv_array.h>
#include <sfa/msg/value_array.h>

namespace sfa {
namespace msg {

class MessageBodyUpdateCodec{
public:
	static ByteArray Encode(const IMessageBody* body, int templateId, IMessageContext* context);
	static bool Decode(ByteArray& buffer, IMessageBody* body, IMessageContext* context, int templateId);

private:
	static ByteArray EncodePMAP(std::vector<int> lstIdx);
	static std::vector<int> DecodePMAP(ByteArray& buffer);

	static ByteArray EncodeArray(const ArrayValue* arrValue);
	static ArrayValue* DecodeArray(ByteArray& buffer, const std::vector<FieldType>& children);

	static ByteArray EncodeKVArray(const KVArrayValue* arrValue);
	static KVArrayValue* DecodeKVArray(ByteArray& buffer, const std::map<int, FieldType>& kvchildren);
};

}
}