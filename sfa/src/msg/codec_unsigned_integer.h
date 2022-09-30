#pragma once
#include "codec_integer.h"

namespace sfa {
namespace msg {

class UnsignedIntegerCodec : public IntegerCodec{
public:
	virtual IFieldValue* Decode(ByteArray& buffer) override;

protected:
	virtual ByteArray EncodeValue(const IFieldValue* value) override;
	ByteArray EncodeValue(uint64_t value);
};

}
}