#pragma once
#include "codec_integer.h"

namespace sfa {
namespace msg {

class SignedIntegerCodec : public IntegerCodec{
public:
	virtual ByteArray EncodeValue(const IFieldValue* value) override;
	virtual IFieldValue* Decode(ByteArray& buffer) override;

protected:
	ByteArray EncodeValue(int64_t longValue);
};

}
}