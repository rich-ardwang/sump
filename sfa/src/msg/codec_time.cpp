#include "codec_time.h"
#include <assert.h>
#include <sfa/msg/value_time.h>
#include <sfa/msg/value_unsigned_integer.h>
#include "codec_util.h"

namespace sfa {
namespace msg {

ByteArray TimeCodec::EncodeValue(const IFieldValue* value){
	assert(value != nullptr);
	time_t dt = ((TimeValue*)value)->GetValue();

	int64_t tmp = MessageCodecUtil::TimeToInt(dt);
	UnsignedIntegerCodec codec;
	UnsignedIntegerValue tmpDt(tmp);
	return codec.Encode(&tmpDt);
}

IFieldValue* TimeCodec::Decode(ByteArray& buffer){
	UnsignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	uint64_t time = ((UnsignedIntegerValue*)value)->ToUInt();
	delete value;

	time_t tmp = MessageCodecUtil::IntToTime(time);
	return new TimeValue(tmp);
}

}
}