#include "context.h"

namespace sfa {
namespace dds {

Context::Context() {
	msg_context_ = msg::NewMessageContext();
}

Context::~Context() {
	msg::DeleteMessageContext(&msg_context_);
}


}
}