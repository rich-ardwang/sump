#pragma once
#include "codec_byte.h"

namespace sfa {
namespace msg {

class UnicodeStringCodec : public ByteCodec{
public:
	virtual ByteArray Encode(const IFieldValue* value) override;
	virtual IFieldValue* Decode(ByteArray& buffer) override;
};

}
}