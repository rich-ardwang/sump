#pragma once 
#include <sfa/msg/msg_codec.h>
#include <assert.h>
#include "codec_msg.h"

namespace sfa {
namespace msg {

bool EncodeMessage(_sfa_in_ const IMessage* message, _sfa_in_ IMessageContext* context, _sfa_out_ uint8_t*& buffer, _sfa_out_ uint32_t& len) {
	assert(message != nullptr);
	assert(context != nullptr);
	assert(buffer == nullptr);
	if (message == nullptr
		|| context == nullptr
		|| buffer != nullptr) {
		return false;
	}

	ByteArray byte_array;
	bool ret = EncodeMessage(message, context, byte_array);
	if (!ret || byte_array.empty()) {
		return false;
	}

	len = byte_array.size();
	buffer = new(std::nothrow) uint8_t[len];
	if (buffer == nullptr) {
		return false;
	}

	for (size_t i=0; i<byte_array.size(); i++) {
		buffer[i] = byte_array[i];
	}
	return true;
}

bool EncodeMessage(_sfa_in_ const IMessage* message, _sfa_in_ IMessageContext* context, _sfa_out_ ByteArray& byte_array) {
	assert(message != nullptr);
	assert(context != nullptr);
	if (message == nullptr
		|| context == nullptr) {
		return false;
	}

	byte_array = MessageCodec::Encode(message, context);
	return true;
}

bool DecodeMessage(_sfa_in_ const uint8_t* buffer, _sfa_in_ uint32_t len, _sfa_in_ IMessageContext* context, _sfa_out_ IMessage*& message){
	assert(buffer!=nullptr && len>0);
	assert(context != nullptr);
	assert(message == nullptr);
	if (buffer==nullptr
		|| len<=0
		|| context==nullptr
		|| message!=nullptr) {
		return false;
	}

	ByteArray byte_array;
	for (uint32_t i=0; i<len; i++) {
		byte_array.push_back(buffer[i]);
	}

	return DecodeMessage(byte_array, context, message);
}

bool DecodeMessage(_sfa_in_ const ByteArray& byte_array, _sfa_in_ IMessageContext* context, _sfa_out_ IMessage*& message) {
	assert(context != nullptr);
	assert(message == nullptr);
	if (context == nullptr
		|| message!=nullptr) {
		return false;
	}

	return MessageCodec::Decode(const_cast<ByteArray&>(byte_array), context, message);
}

}
}
