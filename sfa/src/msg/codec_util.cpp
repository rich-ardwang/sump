#include "codec_util.h"
#include <assert.h>
#include <sfa/msg/value_integer.h>
#include <sfa/msg/value_unsigned_integer.h>
#include <sfa/msg/value_decimal.h>
#include <sfa/msg/value_ascii.h>
#include <sfa/msg/value_unicode.h>
#include <sfa/msg/value_byte.h>
#include <sfa/msg/value_date.h>
#include <sfa/msg/value_time.h>
#include "consts.h"
#include "codec_signed_integer.h"
#include "codec_unsigned_integer.h"
#include "codec_decimal.h"
#include "codec_ascii_string.h"
#include "codec_unicode_string.h"
#include "codec_byte.h"
#include "codec_date.h"
#include "codec_time.h"

namespace sfa {
namespace msg {

Byte MessageCodecUtil::GetFieldTypeValue(Byte fieldInfo){
	//字段类型[0-4]
	Byte bFieldType = (Byte)(fieldInfo >> 3 & 0x1F);//0x1F=0001 1111
	return bFieldType;
}

Byte MessageCodecUtil::GetFieldTypeDescValue(Byte fieldInfo){
	//字段描述[5-7]
	Byte bFieldDesc = (Byte)(fieldInfo & 0x07);//0x07=0000 0111
	return bFieldDesc;
}

Byte MessageCodecUtil::GenFieldInfo(FieldType type, FieldDesc desc){
	//字段类型[0-4]
	Byte fieldType = MessageCodecUtil::GetFieldType(type);
	//字段描述[5-7]
	Byte fieldDesc = MessageCodecUtil::GetFieldDesc(desc);

	return GenFieldInfo(fieldType, fieldDesc);
}

Byte MessageCodecUtil::GenFieldInfo(Byte type, FieldDesc desc){
	return GenFieldInfo(type, MessageCodecUtil::GetFieldDesc(desc));
}

Byte MessageCodecUtil::GenFieldInfo(Byte type, Byte desc){
	Byte fieldInfo = 0x0;// 字段描述

	fieldInfo |= (Byte)(type << 3 & 0xF8); //0xF8=1111 1000
	fieldInfo |= (Byte)(desc & 0x07); //0x07=0000 0111

	return fieldInfo;
}

ByteArray MessageCodecUtil::EncodeFieldValue(FieldType type, const IFieldValue* value){
	assert(value != nullptr);
	ByteArray btBuffer;
	TypeCodec* codec = MessageCodecUtil::GetTypeCodec(type);
	if (codec != nullptr){
		btBuffer = codec->Encode(value);
		delete codec;
	}
	return btBuffer;
}

IFieldValue* MessageCodecUtil::DecodeFieldValue(FieldType type, ByteArray& buffer){
	IFieldValue* value = nullptr;
	TypeCodec* codec = MessageCodecUtil::GetTypeCodec(type);
	if (codec != nullptr){
		value = codec->Decode(buffer);
		delete codec;
	}
	return value;
}

TypeCodec* MessageCodecUtil::GetTypeCodec(FieldType type){
	TypeCodec *codec = nullptr;

	switch (type){
	case FieldType::INTEGER:
		codec = new SignedIntegerCodec;
		break;
	case FieldType::UNSIGNED_INTEGER:
		codec = new UnsignedIntegerCodec;
		break;
	case FieldType::DECIMAL:
		codec = new DecimalCodec;
		break;
	case FieldType::ASCII:
		codec = new AsciiStringCodec;
		break;
	case FieldType::UNIC0DE:
		codec = new UnicodeStringCodec;
		break;
	case FieldType::BYTE:
		codec = new ByteCodec;
		break;
	case FieldType::DATE:
		codec = new DateCodec;
		break;
	case FieldType::TIME:
		codec = new TimeCodec;
		break;
	case FieldType::ARRAY:
		break;
	case FieldType::KV_ARRAY:
		break;
	default:
		assert(0);
		break;
	}

	return codec;
}

IFieldValue* MessageCodecUtil::GetValue(FieldType type, FieldDesc desc, ByteArray& input){
	IFieldValue* value = nullptr;

	if (desc == FieldDesc::NORMAL){
		TypeCodec* codec = GetTypeCodec(type);
		if (codec != nullptr){
			value = codec->Decode(input);
			delete codec;
		}
	}
	else{
		switch (type) {
		case FieldType::INTEGER:
			value = new IntegerValue(desc);
			break;
		case FieldType::UNSIGNED_INTEGER:
			value = new UnsignedIntegerValue(desc);
			break;
		case FieldType::DECIMAL:
			value = new DecimalValue(desc);
			break;
		case FieldType::ASCII:
			value = new AsciiValue(desc);
			break;
		case FieldType::UNIC0DE:
			value = new UnicodeValue(desc);
			break;
		case FieldType::BYTE:
			value = new ByteValue(desc);
			break;
		case FieldType::DATE:
			value = new DateValue(desc);
			break;
		case FieldType::TIME:
			value = new TimeValue(desc);
			break;
		case FieldType::ARRAY:
			break;
		case FieldType::KV_ARRAY:
			break;
		}
	}

	return value;
}

Byte MessageCodecUtil::GetFieldType(FieldType type){
	Byte fieldType = 0;

	switch (type){
	case FieldType::INTEGER:
		fieldType = FIELDTYPE_INT64;
		break;
	case FieldType::UNSIGNED_INTEGER:
		fieldType = FIELDTYPE_UINT64;
		break;
	case FieldType::DECIMAL:
		fieldType = FIELDTYPE_DECIMAL;
		break;
	case FieldType::ASCII:
		fieldType = FIELDTYPE_ASCII;
		break;
	case FieldType::UNIC0DE:
		fieldType = FIELDTYPE_UNICODE;
		break;
	case FieldType::BYTE:
		fieldType = FIELDTYPE_BYTE;
		break;
	case FieldType::DATE:
		fieldType = FIELDTYPE_DATE;
		break;
	case FieldType::TIME:
		fieldType = FIELDTYPE_TIME;
		break;
	case FieldType::ARRAY:
		fieldType = FIELDTYPE_ARRAY_BEGIN;
		break;
	case FieldType::KV_ARRAY:
		fieldType = FIELDTYPE_KVARRAY_BEGIN;
		break;
	default:
		assert(0);
		break;
	}

	return fieldType;
}

FieldType MessageCodecUtil::GetFieldType(Byte bFieldType){
	FieldType type;

	switch (bFieldType) {
	case FIELDTYPE_INT64:
		type = FieldType::INTEGER;
		break;
	case FIELDTYPE_UINT64:
		type = FieldType::UNSIGNED_INTEGER;
		break;
	case FIELDTYPE_DECIMAL:
		type = FieldType::DECIMAL;
		break;
	case FIELDTYPE_ASCII:
		type = FieldType::ASCII;
		break;
	case FIELDTYPE_UNICODE:
		type = FieldType::UNIC0DE;
		break;
	case FIELDTYPE_BYTE:
		type = FieldType::BYTE;
		break;
	case FIELDTYPE_DATE:
		type = FieldType::DATE;
		break;
	case FIELDTYPE_TIME:
		type = FieldType::TIME;
		break;
	case FIELDTYPE_ARRAY_BEGIN:
		type = FieldType::ARRAY;
		break;
	case FIELDTYPE_KVARRAY_BEGIN:
		type = FieldType::KV_ARRAY;
		break;
	default:
		assert(0);
		break;
	}

	return type;
}

Byte MessageCodecUtil::GetFieldDesc(FieldDesc desc){
	Byte fieldDesc = FIELDVALUE_RESERVED;
	switch (desc) {
	case FieldDesc::NORMAL:
		fieldDesc = FIELDVALUE_NORMAL;
		break;
	case FieldDesc::Null:
		fieldDesc = FIELDVALUE_NULL;
		break;
	case FieldDesc::PAID:
		fieldDesc = FIELDVALUE_PAID;
		break;
	case FieldDesc::CORRECT:
		fieldDesc = FIELDVALUE_CORRECT;
		break;
	case FieldDesc::WITHDRAW:
		fieldDesc = FIELDVALUE_WITHDRAW;
		break;
	case FieldDesc::RESERVED:
		fieldDesc = FIELDVALUE_RESERVED;
		break;
	default:
		assert(0);
		break;
	}

	return fieldDesc;
}

FieldDesc MessageCodecUtil::GetFieldDesc(Byte fieldDesc){
	FieldDesc desc;
	switch (fieldDesc){
	case FIELDVALUE_RESERVED:
		desc = FieldDesc::RESERVED;
		break;
	case FIELDVALUE_NORMAL:
		desc = FieldDesc::NORMAL;
		break;
	case FIELDVALUE_NULL:
		desc = FieldDesc::Null;
		break;
	case FIELDVALUE_PAID:
		desc = FieldDesc::PAID;
		break;
	case FIELDVALUE_CORRECT:
		desc = FieldDesc::CORRECT;
		break;
	case FIELDVALUE_WITHDRAW:
		desc = FieldDesc::WITHDRAW;
		break;
	default:
		assert(0);
		break;
	}
	return desc;
}

int64_t MessageCodecUtil::DateToInt(time_t t) {
	//转换成1970年以来的天数
	return t / 60 / 60 / 24; 
}

time_t MessageCodecUtil::IntToDate(int64_t val) {
	return val * 60 * 60 * 24;
}

int64_t MessageCodecUtil::TimeToInt(time_t t) {
	return (int64_t)t;
}

time_t MessageCodecUtil::IntToTime(int64_t val) {
	return (time_t)val;
}

}
}