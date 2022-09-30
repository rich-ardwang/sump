#include "codec_decimal.h"
#include <assert.h>
#include <sfa/msg/value_decimal.h>
#include <sfa/msg/value_integer.h>

namespace sfa {
namespace msg {

IFieldValue* DecimalCodec::Decode(ByteArray& buffer){
	assert(!buffer.empty());

	SignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	int exponent = (int)value->ToInt();
	delete value;
	value = nullptr;

	if (abs(exponent) > 63){
		return nullptr;
	}

	value = codec.Decode(buffer);
	int64_t mantissa = value->ToInt();
	delete value;

	return new DecimalValue((double)mantissa, exponent);
}

ByteArray DecimalCodec::EncodeValue(const IFieldValue* value){
	assert(value != nullptr);
	DecimalValue* v = (DecimalValue*)value;

	if (abs(v->GetExponent()) > 63){
		return ByteArray();
	}

	SignedIntegerCodec codec;
	IntegerValue tmpExp(v->GetExponent());
	ByteArray btExponent = codec.Encode(&tmpExp);
	IntegerValue tmpMan((long)v->GetMantissa());
	ByteArray btMantissa = codec.Encode(&tmpMan);

	ByteArray encoding;
	encoding.insert(encoding.end(), btExponent.begin(), btExponent.end());
	encoding.insert(encoding.end(), btMantissa.begin(), btMantissa.end());
	return encoding;
}

}
}