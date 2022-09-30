#include <sfa/msg/value_unicode.h>
#include <assert.h>

namespace sfa {
namespace msg {

UnicodeValue::UnicodeValue(FieldDesc desc){
	desc_ = desc;
	value_ = "";
}

UnicodeValue::UnicodeValue(std::string value){
	value_ = value;
}

IFieldValue* UnicodeValue::Clone(void){
	return new UnicodeValue(*this);
}

FieldType UnicodeValue::GetFieldType(void) const{
	return FieldType::UNIC0DE;
}

FieldDesc UnicodeValue::GetFieldDesc(void) const{
	return desc_;
}

int64_t UnicodeValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t UnicodeValue::ToUInt(void) {
	assert(0);
	return 0;
}

std::string UnicodeValue::ToString(void){
	return value_;
}

ByteArray UnicodeValue::ToBytes(void){
	ByteArray byteArray;
	for (size_t i=0; i<value_.length(); i++){
		byteArray.push_back((Byte)value_.at(i));
	}
	return byteArray;
}

}
}