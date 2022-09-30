#include <sfa/msg/value_decimal.h>
#include <math.h>
#include <sstream> 
#include <assert.h>

namespace sfa {
namespace msg {

DecimalValue::DecimalValue(FieldDesc desc){
	desc_ = desc;
	exponent_ = 0;
	mantissa_ = 0.0;
}

DecimalValue::DecimalValue(double value){
	if (value == 0.0){
		exponent_ = 0;
		mantissa_ = 0;
		return;
	}

	char buf[64] = { 0 };
	sprintf(buf, "%e", value);
	std::string str(buf);

	auto pos = str.find_first_of("e", 0);
	if (pos != std::string::npos){
		mantissa_ = std::stof(str.substr(0, pos));
		exponent_ = std::stoi(str.substr(pos + 1, str.length()));
	}
}

DecimalValue::DecimalValue(double mantissa, int exponent){
	mantissa_ = mantissa;
	exponent_ = exponent;
}

FieldType DecimalValue::GetFieldType(void) const{
	return FieldType::DECIMAL;
}

FieldDesc DecimalValue::GetFieldDesc(void) const{
	return desc_;
}

int64_t DecimalValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t DecimalValue::ToUInt(void) {
	assert(0);
	return 0;
}

ByteArray DecimalValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

std::string DecimalValue::ToString(void){
	return std::to_string(ToDouble());
}

double DecimalValue::ToDouble(void){
	return mantissa_ * pow(10.0, exponent_);
}

double DecimalValue::GetMantissa(void) const{
	return mantissa_;
}

int DecimalValue::GetExponent(void) const{
	return exponent_;
}

IFieldValue* DecimalValue::Clone(void){
	return new DecimalValue(*this);
}

}
}