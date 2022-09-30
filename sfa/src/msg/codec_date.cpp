#include "codec_date.h"
#include <assert.h>
#include <sfa/msg/value_date.h>
#include <sfa/msg/value_unsigned_integer.h>
#include "codec_util.h"

namespace sfa {
namespace msg {

ByteArray DateCodec::EncodeValue(const IFieldValue* value){
	assert(value != nullptr);
	time_t date = ((DateValue*)value)->GetValue();
	int64_t tmp = MessageCodecUtil::DateToInt(date);
	UnsignedIntegerValue tmpDt(tmp);
	UnsignedIntegerCodec codec;
	return codec.Encode(&tmpDt);
}

IFieldValue* DateCodec::Decode(ByteArray& buffer){
	assert(!buffer.empty());
	UnsignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	uint64_t date = ((UnsignedIntegerValue*)value)->ToUInt();
	delete value;

	time_t tmp = MessageCodecUtil::IntToDate(date);
	return new DateValue(tmp);
}

}
}