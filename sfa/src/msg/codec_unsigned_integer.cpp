#include "codec_unsigned_integer.h"
#include <assert.h>
#include <sfa/msg/value_unsigned_integer.h>

namespace sfa {
namespace msg {

ByteArray UnsignedIntegerCodec::EncodeValue(const IFieldValue* value){
	assert(value != nullptr);
	uint64_t ret = const_cast<IFieldValue*>(value)->ToUInt();
	return EncodeValue(ret);
}

ByteArray UnsignedIntegerCodec::EncodeValue(uint64_t value){
	int size = GetUnsignedIntegerSize(value);
	ByteArray encoded;
	encoded.resize(size);
	for (int factor = 0; factor < size; factor++){
		encoded[size - factor - 1] = (Byte)((value >> (factor * 7)) & 0x7f); //0x7f=1111111
	}
	return encoded;
}

IFieldValue* UnsignedIntegerCodec::Decode(ByteArray& buffer){
	assert(!buffer.empty());

	int byt = 0;
	int value = 0;

	do{
		byt = buffer[0];
		buffer.erase(buffer.begin());
		value = (value<<7)|(byt&0x7f);
	}while ((byt&0x80)==0 && !buffer.empty());

	return new UnsignedIntegerValue(value);
}

}
}