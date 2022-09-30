#include <sfa/msg/value_time.h>
#include <string.h>
#include <time.h>
#include <assert.h>

namespace sfa {
namespace msg {

TimeValue::TimeValue(FieldDesc desc){
	desc_ = desc;
	memset(&value_, 0, sizeof(value_));
}

TimeValue::TimeValue(time_t value){
	value_ = value;
}

TimeValue::TimeValue(int hour, int min, int sec){
	struct tm stm;
	stm.tm_hour = hour;
	stm.tm_min = min;
	stm.tm_sec = sec;
	value_ = mktime(&stm);
}

FieldType TimeValue::GetFieldType(void) const{
	return FieldType::TIME;
}

FieldDesc TimeValue::GetFieldDesc(void) const{
	return desc_;
}

time_t TimeValue::GetValue(void) const{
	return value_;
}

IFieldValue* TimeValue::Clone(void){
	return new TimeValue(*this);
}

int64_t TimeValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t TimeValue::ToUInt(void) {
	assert(0);
	return 0;
}

ByteArray TimeValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

std::string TimeValue::ToString(void){
	struct tm *p = localtime(&value_); 
	if (p == nullptr){
		return "";
	}
	char time[16] = { 0 };
	sprintf(time, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
	time[strlen(time) + 1] = '\0';
	return std::string(time);
}

}
}