#include <sfa/msg/value_ascii.h>
#include <assert.h>

namespace sfa {
namespace msg {

AsciiValue::AsciiValue(FieldDesc desc){
	desc_ = desc;
	value_ = "\0";
}

AsciiValue::AsciiValue(std::string value){
	if (value == ""){
		value_ = "\0";
	}
	else {
		value_ = value;
	}
}

IFieldValue* AsciiValue::Clone(void){
	return new AsciiValue(*this);
}

FieldType AsciiValue::GetFieldType(void) const{
	return FieldType::ASCII;
}

FieldDesc AsciiValue::GetFieldDesc(void) const{
	return desc_;
}

int64_t AsciiValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t AsciiValue::ToUInt(void) {
	assert(0);
	return 0;
}

std::string AsciiValue::ToString(void){
	return value_;
}

ByteArray AsciiValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

}
}