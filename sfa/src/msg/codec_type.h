#pragma once
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class TypeCodec{
public:
	virtual ByteArray Encode(const IFieldValue* value);
	virtual IFieldValue* Decode(ByteArray& buffer) = 0;

protected:
	virtual ByteArray EncodeValue(const IFieldValue* value) = 0;
};

}
}
