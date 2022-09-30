#include "codec_byte.h"
#include <sfa/msg/value_integer.h>
#include <sfa/msg/value_unsigned_integer.h>
#include <sfa/msg/value_byte.h>

namespace sfa {
namespace msg {

ByteArray ByteCodec::Encode(const IFieldValue* value){
	ByteArray bytes = const_cast<IFieldValue*>(value)->ToBytes();

	int lengthSize = IntegerCodec::GetUnsignedIntegerSize(bytes.size());
	UnsignedIntegerCodec codec;
	UnsignedIntegerValue tmpSize(bytes.size());
	ByteArray length = codec.Encode(&tmpSize);

	ByteArray encoding;
	encoding.resize(lengthSize + bytes.size());//size+ÄÚÈÝ
	encoding.insert(encoding.end(), length.begin(), length.end());
	encoding.insert(encoding.end(), bytes.begin(), bytes.end());
	return encoding;
}

IFieldValue* ByteCodec::Decode(ByteArray& buffer){
	UnsignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	uint64_t len = ((UnsignedIntegerValue*)value)->ToUInt();
	delete value;

	ByteArray decoding;
	decoding.resize((size_t)len);

	for (size_t i=0; i<(size_t)len; i++){
		if (!buffer.empty()){
			decoding[i] = buffer[0];
			buffer.erase(buffer.begin());
		}
	}
	return new ByteValue(decoding);
}

}
}