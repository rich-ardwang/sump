#include <sfa/msg/value_byte.h>
#include <assert.h>
#include "utils_byte_hex_str.h"

namespace sfa {
namespace msg {

ByteValue::ByteValue(FieldDesc desc){
	desc_ = desc;
}

ByteValue::ByteValue(ByteArray value){
	value_ = value;
}

IFieldValue* ByteValue::Clone(void){
	return new ByteValue(*this);
}

FieldType ByteValue::GetFieldType(void) const{
	return FieldType::BYTE;
}

FieldDesc ByteValue::GetFieldDesc(void) const{
	return desc_;
}

int64_t ByteValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t ByteValue::ToUInt(void) {
	assert(0);
	return 0;
}

std::string ByteValue::ToString(void){
	std::string src;
	for (size_t i=0; i<value_.size(); i++) {
		src.push_back(value_[i]);
	}
	if (src.empty()) {
		return "";
	}

	return src;
	//std::string dest = utils::HexToStr(src);
	//return dest;
}

ByteArray ByteValue::ToBytes(void){
	return value_;
}

}
}