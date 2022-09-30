#pragma once 
#include <sfa/msg/msg.h>
#include <sfa/msg/msg_context.h>

namespace sfa {
namespace msg {

class MessageCodec{
public:
	static ByteArray Encode(const IMessage* message, IMessageContext* context);
	static bool Decode(ByteArray& buffer, IMessageContext* context, IMessage*& message);
};

}
}
