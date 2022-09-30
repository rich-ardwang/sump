#include "codec_msg_body_full.h"
#include <assert.h>
#include <sfa/msg/value_unsigned_integer.h>
#include "consts.h"
#include "codec_util.h"
#include "codec_unsigned_integer.h"

namespace sfa {
namespace msg {

ByteArray MessageBodyFullCodec::Encode(const IMessageBody* body){
	assert(body != nullptr);
	ByteArray btBuffer;
	for (int fid : body->GetAllFieldId()){
		IFieldValue* value = body->GetField(fid);
		if (value != nullptr){
			ByteArray btField = EncodeField(fid, value);
			btBuffer.insert(btBuffer.end(), btField.begin(), btField.end());
		}
	}
	return btBuffer;
}

ByteArray MessageBodyFullCodec::EncodeField(int fid, IFieldValue* value){
	assert(value != nullptr);

	FieldType fieldType = value->GetFieldType();
	FieldDesc fieldDesc = value->GetFieldDesc();

	//FID��Ϣ
	Byte fieldInfo = MessageCodecUtil::GenFieldInfo(fieldType, fieldDesc);

	//FID
	UnsignedIntegerCodec codec;
	UnsignedIntegerValue tmpFid(fid);
	ByteArray btFID = codec.Encode(&tmpFid);

	//ֵ
	ByteArray btValue;
	if (fieldType == FieldType::ARRAY){// ���鵥������
		btValue = EncodeArray((ArrayValue*)value);
	}
	else if (fieldType == FieldType::KV_ARRAY){// key-value���鵥������
		btValue = EncodeKVArray((KVArrayValue*)value);
	}
	else{
		// �ֶ����ݣ�ֻ���ֶ���ֵ��ת��������Ҫת��ռλ��
		if (fieldDesc == FieldDesc::NORMAL){
			btValue = MessageCodecUtil::EncodeFieldValue(fieldType, value);
		}
	}

	ByteArray btBuffer;
	btBuffer.push_back(fieldInfo);
	btBuffer.insert(btBuffer.end(), btFID.begin(), btFID.end());
	btBuffer.insert(btBuffer.end(), btValue.begin(), btValue.end());
	return btBuffer;
}

ByteArray MessageBodyFullCodec::EncodeArray(ArrayValue* arrValue){
	assert(arrValue != nullptr);
	ByteArray btBuffer;

	//���ɱ�ͷ
	for (FieldType type : arrValue->GetColTypes()){
		btBuffer.push_back(MessageCodecUtil::GenFieldInfo(type, FieldDesc::RESERVED));
	}

	//����������
	for (ArrayRowValue* row : arrValue->GetRows()){
		if (row != nullptr){
			//���п�ʼ
			btBuffer.push_back(MessageCodecUtil::GenFieldInfo(FIELDTYPE_ARRAY_NEWROW,
				FieldDesc::RESERVED));

			for (IFieldValue* value : row->GetValues()){
				if (value != nullptr){
					ByteArray btValue = MessageCodecUtil::EncodeFieldValue(value->GetFieldType(), value);
					btBuffer.insert(btBuffer.end(), btValue.begin(), btValue.end());
				}
			}
		}
	}

	//�������
	btBuffer.push_back(MessageCodecUtil::GenFieldInfo(FIELDTYPE_ARRAY_END,
		FieldDesc::RESERVED));
	return btBuffer;
}

ByteArray MessageBodyFullCodec::EncodeKVArray(const KVArrayValue* kvArrValue){
	assert(kvArrValue != nullptr);
	ByteArray btBuffer;

	//���ɱ�ͷ
	for (int fid : kvArrValue->GetColAllFids()) {
		//type
		FieldType type = kvArrValue->GetColType(fid);
		btBuffer.push_back(MessageCodecUtil::GenFieldInfo(type, FieldDesc::RESERVED));

		//FID
		UnsignedIntegerValue tmpFid(fid);
		UnsignedIntegerCodec codec;
		ByteArray btFID = codec.Encode(&tmpFid);
		btBuffer.insert(btBuffer.end(), btFID.begin(), btFID.end());
	}

	//����������
	for (KVArrayRowValue* row : kvArrValue->GetRows()){
		if (row != nullptr){
			//���п�ʼ
			btBuffer.push_back(MessageCodecUtil::GenFieldInfo(FIELDTYPE_KVARRAY_NEWROW,
				FieldDesc::RESERVED));

			for (int fid : kvArrValue->GetColAllFids()){
				IFieldValue* value = row->GetValue(fid);
				if (value == nullptr){//��FIDû������
					btBuffer.push_back(MessageCodecUtil::GenFieldInfo((Byte)0, FieldDesc::RESERVED));
				}
				else {
					btBuffer.push_back(MessageCodecUtil::GenFieldInfo((Byte)1, value->GetFieldDesc()));
					if (value->GetFieldDesc() == FieldDesc::NORMAL) {
						ByteArray btValue = MessageCodecUtil::EncodeFieldValue(value->GetFieldType(), value);
						btBuffer.insert(btBuffer.end(), btValue.begin(), btValue.end());
					}
				}
			}
		}
	}

	//�������
	btBuffer.push_back(MessageCodecUtil::GenFieldInfo(FIELDTYPE_KVARRAY_END,
		FieldDesc::RESERVED));
	return btBuffer;
}

bool MessageBodyFullCodec::Decode(ByteArray& buffer, IMessageBody* body){
	assert(body != nullptr);
	while (!buffer.empty()){
		int byt = buffer[0];
		buffer.erase(buffer.begin());
		FieldValueTypeDesc valueTypeOpt = GetFieldInfo(byt);

		//FID
		UnsignedIntegerCodec codec;
		IFieldValue *fid = codec.Decode(buffer);
		assert(fid != nullptr);

		//Value
		IFieldValue* value = nullptr;
		if (valueTypeOpt.type_ == FieldType::ARRAY){
			value = DecodeArray(buffer);
		}
		else if (valueTypeOpt.type_ == FieldType::KV_ARRAY){
			value = DecodeKVArray(buffer);
		}
		else{
			value = MessageCodecUtil::GetValue(valueTypeOpt.type_, valueTypeOpt.desc_, buffer);
		}
		assert(value != nullptr);

		body->AddField((int)fid->ToUInt(), value);

		if (fid != nullptr){
			delete fid;
			fid = nullptr;
		}
		if (value != nullptr){
			delete value;
			value = nullptr;
		}
	}
	return true;
}

ArrayValue* MessageBodyFullCodec::DecodeArray(ByteArray& buffer){
	ArrayValue* arrValue = new(std::nothrow) ArrayValue;
	assert(arrValue != nullptr);

	bool isEnd = false;
	bool isCol = true;
	ArrayRowValue* row = nullptr;

	while (!isEnd && !buffer.empty()) {
		Byte byt = buffer[0];
		buffer.erase(buffer.begin());

		ArrayFlag arrayFlag = GetArrayFlag(byt);

		switch (arrayFlag){
		case NORMAL:
			if (isCol) {//������ͷ
				FieldValueTypeDesc valueTypeOpt = GetFieldInfo(byt);
				arrValue->AddColType(valueTypeOpt.type_);
			}
			break;
		case NEW_ROW:
			isCol = false;
			row = arrValue->AddRow();
			assert(row != nullptr);
			for (FieldType type : arrValue->GetColTypes()) {//��������
				IFieldValue* value = MessageCodecUtil::GetValue(type, FieldDesc::NORMAL, buffer);
				if (value != nullptr){
					row->AppendValue(value);
					delete value;
				}
			}
			break;
		case END:
			isEnd = true;
			break;
		}
	}

	return arrValue;
}

KVArrayValue* MessageBodyFullCodec::DecodeKVArray(ByteArray& buffer){
	KVArrayValue* arrValue = new(std::nothrow) KVArrayValue;
	assert(arrValue != nullptr);

	bool isEnd = false;
	bool isCol = true;
	KVArrayRowValue* row = nullptr;

	while (!isEnd && !buffer.empty()) {
		Byte byt = buffer[0];
		buffer.erase(buffer.begin());

		ArrayFlag arrayFlag = GetArrayFlag(byt);
		switch (arrayFlag){
		case NORMAL:
			if (isCol){//������ͷ
				FieldValueTypeDesc valueTypeOpt = GetFieldInfo(byt);
				UnsignedIntegerCodec codec;
				IFieldValue* fid = codec.Decode(buffer);//��������
				arrValue->SetColType((int)fid->ToUInt(), valueTypeOpt.type_);
				delete fid;
			}
			break;
		case NEW_ROW:
			isCol = false;
			row = arrValue->AddRow();
			assert(row != nullptr);
			for (int fid : arrValue->GetColAllFids()){//��������
				if (!buffer.empty()){
					byt = buffer[0];//�����Ƿ���ڵ�������Ϣ
					buffer.erase(buffer.begin());

					ByteArray valueTypeOpt = GetFieldInfoByte(byt);
					if (valueTypeOpt.size()==2 && valueTypeOpt[0] == 1) {//����ֵ�Ŷ�ȡ
						FieldType type = arrValue->GetColType(fid);
						IFieldValue* value = MessageCodecUtil::GetValue(type, MessageCodecUtil::GetFieldDesc(valueTypeOpt[1]), buffer);
						if (value != nullptr){
							row->SetValue(fid, value);
							delete value;
						}
					}
				}
			}
			break;
		case END:
			isEnd = true;
			break;
		}
	}

	return arrValue;
}

MessageBodyFullCodec::ArrayFlag MessageBodyFullCodec::GetArrayFlag(Byte fieldInfo){
	//�ֶ�����[0-4]
	Byte bFieldType = MessageCodecUtil::GetFieldTypeValue(fieldInfo);

	//�ֶ�����
	switch (bFieldType){
	case FIELDTYPE_ARRAY_NEWROW:
		return ArrayFlag::NEW_ROW;
	case FIELDTYPE_KVARRAY_NEWROW:
		return ArrayFlag::NEW_ROW;
	case FIELDTYPE_ARRAY_END:
		return ArrayFlag::END;
	case FIELDTYPE_KVARRAY_END:
		return ArrayFlag::END;
	default:
		return ArrayFlag::NORMAL;
	}
}

MessageBodyFullCodec::FieldValueTypeDesc MessageBodyFullCodec::GetFieldInfo(Byte fieldInfo){
	ByteArray btInfo = GetFieldInfoByte(fieldInfo);
	FieldType type = MessageCodecUtil::GetFieldType(btInfo[0]);
	FieldDesc opt = MessageCodecUtil::GetFieldDesc(btInfo[1]);

	return FieldValueTypeDesc(type, opt);
}

ByteArray MessageBodyFullCodec::GetFieldInfoByte(Byte fieldInfo){
	ByteArray btBuffer;

	//�ֶ�����[0-4]
	Byte bFieldType = MessageCodecUtil::GetFieldTypeValue(fieldInfo);
	btBuffer.push_back(bFieldType);

	//�ֶ�����[5-7]
	Byte bFieldDesc = MessageCodecUtil::GetFieldTypeDescValue(fieldInfo);
	btBuffer.push_back(bFieldDesc);

	return btBuffer;
}

}
}
