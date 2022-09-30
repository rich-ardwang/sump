#include <sfa/msg/value_unsigned_integer.h>
#include <assert.h>

namespace sfa {
namespace msg {

UnsignedIntegerValue::UnsignedIntegerValue(FieldDesc desc){
	desc_ = desc;
}

UnsignedIntegerValue::UnsignedIntegerValue(uint64_t value){
	value_ = value;
}

FieldType UnsignedIntegerValue::GetFieldType(void) const{
	return FieldType::UNSIGNED_INTEGER;
}

FieldDesc UnsignedIntegerValue::GetFieldDesc(void) const{
	return desc_;
}

IFieldValue* UnsignedIntegerValue::Clone(void){
	return new UnsignedIntegerValue(*this);
}

std::string UnsignedIntegerValue::ToString(void){
	return std::to_string(value_);
}

int64_t UnsignedIntegerValue::ToInt(void) {
	assert(0);
	return 0;
}

uint64_t UnsignedIntegerValue::ToUInt(void) {
	return value_;
}

ByteArray UnsignedIntegerValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

}
}