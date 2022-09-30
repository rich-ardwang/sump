#pragma once
#include "codec_signed_integer.h"

namespace sfa {
namespace msg {

class DecimalCodec : public SignedIntegerCodec{
public:
	virtual IFieldValue* Decode(ByteArray& buffer) override;

protected:
	virtual ByteArray EncodeValue(const IFieldValue* value) override;
};

}
}