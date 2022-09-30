#pragma once
#include <sfa/msg/msg_header.h>

namespace sfa {
namespace msg {

class MessageHeaderCodec{
public:
	static ByteArray Encode(const IMessageHeader* header);
	static bool Decode(ByteArray& buffer, IMessageHeader* header);

protected:
	static ByteArray EncodeUnsignedIntegerVal(int value);
	static uint64_t DecodeUnsignedIntegerVal(ByteArray& buffer);
};

}
}