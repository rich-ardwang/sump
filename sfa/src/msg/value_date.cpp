#include <sfa/msg/value_date.h>
#include <string.h>
#include <time.h>
#include <assert.h>

namespace sfa{
namespace msg{

DateValue::DateValue(FieldDesc desc){
	desc_ = desc;
	memset(&value_, 0, sizeof(value_));
}

DateValue::DateValue(time_t value){
	value_ = value;
}

DateValue::DateValue(int year, int month, int day){
	struct tm stm;
	stm.tm_year = year - 1900;
	stm.tm_mon = month - 1;
	stm.tm_mday = day;
	value_ = mktime(&stm);
}

FieldType DateValue::GetFieldType(void) const{
	return FieldType::DATE;
}

FieldDesc DateValue::GetFieldDesc(void) const{
	return desc_;
}

time_t DateValue::GetValue(void) const{
	return value_;
}

IFieldValue* DateValue::Clone(void){
	return new DateValue(*this);
}

int64_t DateValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t DateValue::ToUInt(void){
	assert(0);
	return 0;
}

std::string DateValue::ToString(void){
	struct tm *p = localtime(&value_);
	if (p == nullptr){
		return "";
	}
	char date[16] = { 0 };
	sprintf(date, "%04d-%02d-%02d",
		1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
	date[strlen(date) + 1] = '\0';
	return std::string(date);
}

ByteArray DateValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

}
}