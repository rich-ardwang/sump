#pragma once
#include "codec_unsigned_integer.h"

namespace sfa {
namespace msg {

class TimeCodec : public UnsignedIntegerCodec{
public:
	virtual ByteArray EncodeValue(const IFieldValue* value) override;
	virtual IFieldValue* Decode(ByteArray& buffer) override;
};

}
}