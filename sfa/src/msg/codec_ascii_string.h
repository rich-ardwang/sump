#pragma once
#include "codec_type.h"

namespace sfa {
namespace msg {

class AsciiStringCodec : public TypeCodec{
public:
	virtual IFieldValue* Decode(ByteArray& buffer) override;

protected:
	virtual ByteArray EncodeValue(const IFieldValue* value) override;

private:
	static bool IsEmpty(const ByteArray& bytes);
};

}
}