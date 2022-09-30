#include "codec_msg_header.h"
#include <assert.h>
#include <sfa/msg/value_unsigned_integer.h>
#include "consts.h"
#include "codec_unsigned_integer.h"
#include "codec_util.h"
#include "codec_msg_body_full.h"

namespace sfa {
namespace msg {

static Byte MakeFieldStatus(const IMessageHeader* header){
	//字段状态：[0]停止位，[1-2]消息类型，[3]存在模板编号,[4]存在模板版本号,[5]存在附加参数
	Byte fieldStatus = 0x00;

	//停止位（1位）
	fieldStatus |= (Byte)(STOP_YES << 7 & 0x80); //0x80=1000 0000

	//消息类型（2位）
	if (header->GetMessageType() == IMessageHeader::FULL){
		fieldStatus |= (Byte)(MESSAGE_TYPE_FULL << 5 & 0x60); //0x60=0110 0000
	}
	else if (header->GetMessageType() == IMessageHeader::INCREMENTAL){
		fieldStatus |= (Byte)(MESSAGE_TYPE_INCREMENTAL << 5 & 0x60);
	}
	else if (header->GetMessageType() == IMessageHeader::TEMPLATE){
		fieldStatus |= (Byte)(MESSAGE_TYPE_TEMPLATE << 5 & 0x60);
	}

	//存在模板编号（1位）
	if (header->GetTemplateId() != 0){
		fieldStatus |= (Byte)(BYTE_YES << 4 & 0x10); //0x10=0001 0000
	}

	//存在模板版本号（1位）
	if (header->GetVersion() != 0){
		fieldStatus |= (Byte)(BYTE_YES << 3 & 0x08); //0x08=0000 1000
	}

	//存在附加参数（1位）
	if (header->GetExtraFieldSize() > 0){
		fieldStatus |= (Byte)(BYTE_YES << 2 & 0x04); //0x04=0000 0100
	}

	return fieldStatus;
}

ByteArray MessageHeaderCodec::Encode(const IMessageHeader* header){
	assert(header != nullptr);
	ByteArray byteArray;

	//消息固定标示
	byteArray.push_back(MESSAGE_FLAG);

	//状态字段([0]停止位，[1-2]消息类型，[3]存在模板编号,[4]存在模板版本号,[5]存在附加参数)
	byteArray.push_back(MakeFieldStatus(header));

	//模板编号
	if (header->GetTemplateId() != 0) {
		ByteArray templateId = MessageHeaderCodec::EncodeUnsignedIntegerVal(header->GetTemplateId());
		byteArray.insert(byteArray.end(), templateId.begin(), templateId.end());
	}

	//模板版本号
	if (header->GetVersion() != 0) {
		ByteArray version = MessageHeaderCodec::EncodeUnsignedIntegerVal(header->GetVersion());
		byteArray.insert(byteArray.end(), version.begin(), version.end());
	}

	//附加参数
	if (header->GetExtraFieldSize() > 0){
		//记录参数数量
		ByteArray paramSize = MessageHeaderCodec::EncodeUnsignedIntegerVal(header->GetExtraFieldSize());
		byteArray.insert(byteArray.end(), paramSize.begin(), paramSize.end());

		for (int fid : header->GetAllExtraFieldId()){
			IFieldValue* value = header->GetExtraField(fid);
			if (value == nullptr){
				continue;
			}

			//FID信息
			Byte fieldInfo = MessageCodecUtil::GenFieldInfo(value->GetFieldType(), value->GetFieldDesc());
			byteArray.push_back(fieldInfo);
			
			//FID
			ByteArray btFID = MessageHeaderCodec::EncodeUnsignedIntegerVal(fid);
			byteArray.insert(byteArray.end(), btFID.begin(), btFID.end());

			//值
			if (value->GetFieldType() == FieldType::ARRAY
				|| value->GetFieldType() == FieldType::KV_ARRAY) {//数组，key-value数组
				//不支持
				continue;
			}

			//字段内容：只有字段有值才转换，不需要转换占位符
			if (value->GetFieldDesc() == FieldDesc::NORMAL){
				ByteArray btValue = MessageCodecUtil::EncodeFieldValue(value->GetFieldType(), value);//值
				byteArray.insert(byteArray.end(), btValue.begin(), btValue.end());
			}
		}
	}

	return byteArray;
}

bool MessageHeaderCodec::Decode(ByteArray& buffer, IMessageHeader* header){
	assert(!buffer.empty());
	assert(header != nullptr);

	if (buffer.empty()){
		return false;
	}

	int byt = buffer[0];
	buffer.erase(buffer.begin());

	//消息固定标示
	if (byt != MESSAGE_FLAG){
		return false;
	}

	//状态字段
	Byte fieldStatus = (Byte)(MessageHeaderCodec::DecodeUnsignedIntegerVal(buffer) & 0xff); //0xff=1111 1111

	//消息类型
	Byte msgType = (Byte)((fieldStatus & 0x60) >> 5); //0x60=0110 0000
	if (msgType == MESSAGE_TYPE_FULL){
		header->SetMessageType(IMessageHeader::FULL);
	}
	else if (msgType == MESSAGE_TYPE_INCREMENTAL){
		header->SetMessageType(IMessageHeader::INCREMENTAL);
	}
	else if (msgType == MESSAGE_TYPE_TEMPLATE){
		header->SetMessageType(IMessageHeader::TEMPLATE);
	}

	//存在模板编号
	Byte hasTemplateId = (Byte)((fieldStatus & 0x10) >> 4); //0x10=0001 0000

	//存在模板版本号
	Byte hasVersion = (Byte)((fieldStatus & 0x08) >> 3); //0x08=0000 1000

	//存在附加参数
	Byte hasParam = (Byte)((fieldStatus & 0x04) >> 2); //0x04=0000 0100

	if (hasTemplateId == BYTE_YES){
		header->SetTemplateId((int)MessageHeaderCodec::DecodeUnsignedIntegerVal(buffer));
	}

	if (hasVersion == BYTE_YES){
		header->SetVersion((int)MessageHeaderCodec::DecodeUnsignedIntegerVal(buffer));
	}

	if (hasParam == BYTE_YES){
		int paramSize = (int)MessageHeaderCodec::DecodeUnsignedIntegerVal(buffer);

		for (int i=0; i<paramSize; i++){
			if (!buffer.empty()){
				byt = buffer[0];
				buffer.erase(buffer.begin());

				MessageBodyFullCodec::FieldValueTypeDesc valueTypeOpt = MessageBodyFullCodec::GetFieldInfo(byt);
				int fid = (int)MessageHeaderCodec::DecodeUnsignedIntegerVal(buffer);
				IFieldValue* value = nullptr;

				if (valueTypeOpt.type_ == FieldType::ARRAY
					|| valueTypeOpt.type_ == FieldType::KV_ARRAY){
					//不支持
				}
				else{
					value = MessageCodecUtil::GetValue(valueTypeOpt.type_, valueTypeOpt.desc_, buffer);
				}

				if (value != nullptr){
					header->AddExtraField(fid, value);
					delete value;
				}
			}
		}
	}

	return true;
}

ByteArray MessageHeaderCodec::EncodeUnsignedIntegerVal(int value){
	UnsignedIntegerCodec codec;
	UnsignedIntegerValue tmpVal(value);
	return codec.Encode(&tmpVal);
}

uint64_t MessageHeaderCodec::DecodeUnsignedIntegerVal(ByteArray& buffer){
	UnsignedIntegerCodec codec;
	UnsignedIntegerValue* value = (UnsignedIntegerValue*)codec.Decode(buffer);
	uint64_t val = value->ToUInt();
	delete value;
	return val;
}

}
}