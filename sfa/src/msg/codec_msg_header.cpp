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
	//�ֶ�״̬��[0]ֹͣλ��[1-2]��Ϣ���ͣ�[3]����ģ����,[4]����ģ��汾��,[5]���ڸ��Ӳ���
	Byte fieldStatus = 0x00;

	//ֹͣλ��1λ��
	fieldStatus |= (Byte)(STOP_YES << 7 & 0x80); //0x80=1000 0000

	//��Ϣ���ͣ�2λ��
	if (header->GetMessageType() == IMessageHeader::FULL){
		fieldStatus |= (Byte)(MESSAGE_TYPE_FULL << 5 & 0x60); //0x60=0110 0000
	}
	else if (header->GetMessageType() == IMessageHeader::INCREMENTAL){
		fieldStatus |= (Byte)(MESSAGE_TYPE_INCREMENTAL << 5 & 0x60);
	}
	else if (header->GetMessageType() == IMessageHeader::TEMPLATE){
		fieldStatus |= (Byte)(MESSAGE_TYPE_TEMPLATE << 5 & 0x60);
	}

	//����ģ���ţ�1λ��
	if (header->GetTemplateId() != 0){
		fieldStatus |= (Byte)(BYTE_YES << 4 & 0x10); //0x10=0001 0000
	}

	//����ģ��汾�ţ�1λ��
	if (header->GetVersion() != 0){
		fieldStatus |= (Byte)(BYTE_YES << 3 & 0x08); //0x08=0000 1000
	}

	//���ڸ��Ӳ�����1λ��
	if (header->GetExtraFieldSize() > 0){
		fieldStatus |= (Byte)(BYTE_YES << 2 & 0x04); //0x04=0000 0100
	}

	return fieldStatus;
}

ByteArray MessageHeaderCodec::Encode(const IMessageHeader* header){
	assert(header != nullptr);
	ByteArray byteArray;

	//��Ϣ�̶���ʾ
	byteArray.push_back(MESSAGE_FLAG);

	//״̬�ֶ�([0]ֹͣλ��[1-2]��Ϣ���ͣ�[3]����ģ����,[4]����ģ��汾��,[5]���ڸ��Ӳ���)
	byteArray.push_back(MakeFieldStatus(header));

	//ģ����
	if (header->GetTemplateId() != 0) {
		ByteArray templateId = MessageHeaderCodec::EncodeUnsignedIntegerVal(header->GetTemplateId());
		byteArray.insert(byteArray.end(), templateId.begin(), templateId.end());
	}

	//ģ��汾��
	if (header->GetVersion() != 0) {
		ByteArray version = MessageHeaderCodec::EncodeUnsignedIntegerVal(header->GetVersion());
		byteArray.insert(byteArray.end(), version.begin(), version.end());
	}

	//���Ӳ���
	if (header->GetExtraFieldSize() > 0){
		//��¼��������
		ByteArray paramSize = MessageHeaderCodec::EncodeUnsignedIntegerVal(header->GetExtraFieldSize());
		byteArray.insert(byteArray.end(), paramSize.begin(), paramSize.end());

		for (int fid : header->GetAllExtraFieldId()){
			IFieldValue* value = header->GetExtraField(fid);
			if (value == nullptr){
				continue;
			}

			//FID��Ϣ
			Byte fieldInfo = MessageCodecUtil::GenFieldInfo(value->GetFieldType(), value->GetFieldDesc());
			byteArray.push_back(fieldInfo);
			
			//FID
			ByteArray btFID = MessageHeaderCodec::EncodeUnsignedIntegerVal(fid);
			byteArray.insert(byteArray.end(), btFID.begin(), btFID.end());

			//ֵ
			if (value->GetFieldType() == FieldType::ARRAY
				|| value->GetFieldType() == FieldType::KV_ARRAY) {//���飬key-value����
				//��֧��
				continue;
			}

			//�ֶ����ݣ�ֻ���ֶ���ֵ��ת��������Ҫת��ռλ��
			if (value->GetFieldDesc() == FieldDesc::NORMAL){
				ByteArray btValue = MessageCodecUtil::EncodeFieldValue(value->GetFieldType(), value);//ֵ
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

	//��Ϣ�̶���ʾ
	if (byt != MESSAGE_FLAG){
		return false;
	}

	//״̬�ֶ�
	Byte fieldStatus = (Byte)(MessageHeaderCodec::DecodeUnsignedIntegerVal(buffer) & 0xff); //0xff=1111 1111

	//��Ϣ����
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

	//����ģ����
	Byte hasTemplateId = (Byte)((fieldStatus & 0x10) >> 4); //0x10=0001 0000

	//����ģ��汾��
	Byte hasVersion = (Byte)((fieldStatus & 0x08) >> 3); //0x08=0000 1000

	//���ڸ��Ӳ���
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
					//��֧��
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