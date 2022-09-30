#pragma once
#include "codec_unsigned_integer.h"

namespace sfa {
namespace msg {

class ByteCodec : public UnsignedIntegerCodec{
public:
	virtual ByteArray Encode(const IFieldValue* value) override;
	virtual IFieldValue* Decode(ByteArray& buffer) override;
};

}
}