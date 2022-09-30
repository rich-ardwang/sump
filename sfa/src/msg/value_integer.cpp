#include <sfa/msg/value_integer.h>
#include <assert.h>

namespace sfa {
namespace msg {

IntegerValue::IntegerValue(FieldDesc desc){
	desc_ = desc;
}

IntegerValue::IntegerValue(int64_t value){
	value_ = value;
}

FieldType IntegerValue::GetFieldType(void) const{
	return FieldType::INTEGER;
}

FieldDesc IntegerValue::GetFieldDesc(void) const{
	return desc_;
}

IFieldValue* IntegerValue::Clone(void){
	return new IntegerValue(*this);
}

int64_t IntegerValue::ToInt(void){
	return value_;
}

uint64_t IntegerValue::ToUInt(void) {
	assert(0);
	return 0;
}

std::string IntegerValue::ToString(void){
	return std::to_string(value_);
}

ByteArray IntegerValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

}
}