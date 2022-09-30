#include "codec_msg_body_update.h"
#include <algorithm>
#include <assert.h>
#include <sfa/msg/value_unsigned_integer.h>
#include "consts.h"
#include "codec_util.h"
#include "codec_msg_body_full.h"
#include "codec_type.h"
#include "codec_unsigned_integer.h"
#include "field_id_type.h"
#include "msg_context_impl.h"

namespace sfa {
namespace msg {

ByteArray MessageBodyUpdateCodec::Encode(const IMessageBody* body, int templateId, IMessageContext* context){
	assert(body != nullptr);
	assert(templateId != 0);
	assert(context != nullptr);

	MessageTemplate* msgTemplate = ((MessageContext*)context)->GetTemplate(templateId);
	if (msgTemplate == nullptr){
		return ByteArray();
	}

	//���ұ仯������
	std::vector<int> lstIdx;
	std::vector<int> fids;
	for (int fid : body->GetAllFieldId()){
		int idx = msgTemplate->GetFieldIndexByFID(fid);
		if (idx == -1){
			return ByteArray();
		}
		else if (body->GetField(fid)!=nullptr
			&& body->GetField(fid)->GetFieldDesc()==FieldDesc::NORMAL){//ֻ������ֵ�Ŵ���
			lstIdx.push_back(idx);
			fids.push_back(fid);
		}
	}

	ByteArray btBuffer;

	//���ݱ仯���ɴ���ͼ
	ByteArray btPMAP = EncodePMAP(lstIdx);
	if (btPMAP.empty()) {
		return ByteArray();
	}
	btBuffer.insert(btBuffer.end(), btPMAP.begin(), btPMAP.end());

	//��������
	for (int fid : fids){
		IFieldValue* value = body->GetField(fid);
		if (value == nullptr){
			continue;
		}

		ByteArray btValue;
		FieldType fieldType = value->GetFieldType();
		FieldDesc fieldDesc = value->GetFieldDesc();

		if (fieldType == FieldType::ARRAY){ //���鵥������
			btValue = EncodeArray((ArrayValue*)value);
		}
		else if (fieldType == FieldType::KV_ARRAY){ //KV���鵥������
			btValue = EncodeKVArray((KVArrayValue*)value);
		}
		else{
			//�ֶ����ݣ�ֻ���ֶ���ֵ��ת��������Ҫת��ռλ��
			if (fieldDesc == FieldDesc::NORMAL){
				btValue = MessageCodecUtil::EncodeFieldValue(fieldType, value);
			}
		}
		btBuffer.insert(btBuffer.end(), btValue.begin(), btValue.end());
	}

	return btBuffer;
}

ByteArray MessageBodyUpdateCodec::EncodePMAP(std::vector<int> lstIdx){
	ByteArray btPMAP;
	if (lstIdx.size() == 0){
		return btPMAP;
	}

	//������Ҫ���ֽ���(ÿ�ֽڴ�7λ)
	int maxIdx = lstIdx[lstIdx.size() - 1] + 1; //���������ţ�������0��ʼ��ռ��λ����Ҫ+1
	int len = maxIdx / 7;
	if (maxIdx % 7 != 0){ //���ܱ�7����
		len++;
	}

	btPMAP.resize(len);
	for (size_t i = 0; i < lstIdx.size(); i++){
		int idx = lstIdx[i] + 1;
		btPMAP[(idx - 1) / 7] |= (Byte)(idx % 7 == 0 ? 1 : (1 << (7 - idx % 7)));
	}

	btPMAP[btPMAP.size() - 1] |= (Byte)0x80;
	return btPMAP;
}

ByteArray MessageBodyUpdateCodec::EncodeArray(const ArrayValue* arrValue){
	assert(arrValue != nullptr);
	ByteArray btBuffer;

	//������������
	UnsignedIntegerValue tmpRowSize(arrValue->GetRows().size());
	UnsignedIntegerCodec codec;
	ByteArray btSize = codec.Encode(&tmpRowSize);
	btBuffer.insert(btBuffer.end(), btSize.begin(), btSize.end());

	//����������
	for (ArrayRowValue* row : arrValue->GetRows()){
		if (row == nullptr){
			continue;
		}

		for (IFieldValue* value : row->GetValues()){
			if (value != nullptr){
				ByteArray btValue = MessageCodecUtil::EncodeFieldValue(value->GetFieldType(), value);
				btBuffer.insert(btBuffer.end(), btValue.begin(), btValue.end());
			}
		}
	}
	return btBuffer;
}

ByteArray MessageBodyUpdateCodec::EncodeKVArray(const KVArrayValue* arrValue){
	assert(arrValue != nullptr);
	ByteArray btBuffer;

	//������������
	UnsignedIntegerValue tmpRowsSize(arrValue->GetRows().size());
	UnsignedIntegerCodec codec;
	ByteArray btSize = codec.Encode(&tmpRowsSize);
	btBuffer.insert(btBuffer.end(), btSize.begin(), btSize.end());

	//����������
	for (KVArrayRowValue* row : arrValue->GetRows()){
		if (row == nullptr){
			continue;
		}

		//���п�ʼ
		Byte newRow = MessageCodecUtil::GenFieldInfo(FIELDTYPE_KVARRAY_NEWROW,
			FieldDesc::RESERVED); 
		btBuffer.push_back(newRow);

		for (int fid : arrValue->GetColAllFids()){
			IFieldValue* value = row->GetValue(fid);
			if (value == nullptr) {// ��FIDû������
				btBuffer.push_back(MessageCodecUtil::GenFieldInfo((Byte)0, FieldDesc::RESERVED));
			}
			else {
				btBuffer.push_back(MessageCodecUtil::GenFieldInfo((Byte)1, FieldDesc::NORMAL));
				ByteArray btValue = MessageCodecUtil::EncodeFieldValue(value->GetFieldType(), value);
				btBuffer.insert(btBuffer.end(), btValue.begin(), btValue.end());
			}
		}
	}
	return btBuffer;
}

bool MessageBodyUpdateCodec::Decode(ByteArray& buffer, IMessageBody* body, IMessageContext* context, int templateId){
	assert(body != nullptr);
	assert(context != nullptr);

	MessageTemplate* msgTemplate = ((MessageContext*)context)->GetTemplate(templateId);
	if (msgTemplate == nullptr){
		return false;
	}

	//���ݴ���ͼ�ҵ��仯����
	std::vector<int> lstIdx = DecodePMAP(buffer);
	for (int idx : lstIdx){
		FieldIdAndType* fidType = msgTemplate->GetFieldByIndex(idx);
		if (fidType == nullptr){
			continue;
		}

		int fid = fidType->GetFid();
		IFieldValue* value = nullptr;

		FieldType type = fidType->GetType();
		if (type == FieldType::ARRAY){
			value = DecodeArray(buffer, fidType->GetChildren());
		}
		else if (type == FieldType::KV_ARRAY){
			value = DecodeKVArray(buffer, fidType->GetKVChildren());
		}
		else {
			value = MessageCodecUtil::DecodeFieldValue(type, buffer);
		}

		if (value != nullptr){
			body->AddField(fid, value);
			delete value;
		}
	}

	return true;
}

std::vector<int> MessageBodyUpdateCodec::DecodePMAP(ByteArray& buffer){
	std::vector<int> lstIdx;
	if (!buffer.empty()){
		int byt = 0;
		int page = 0;

		do{
			byt = buffer[0];
			buffer.erase(buffer.begin());

			int value = (byt & 0x7f);
			for (int i = 0; i < 7; i++){
				int flag = value >> i & 0x01;
				if (flag == 1){
					int idx = 7 * page + (7 - i) - 1;
					lstIdx.push_back(idx);
				}
			}
			page++;
		} while ((byt & 0x80) == 0);

		std::sort(lstIdx.begin(), lstIdx.end());
	}

	return lstIdx;
}

ArrayValue* MessageBodyUpdateCodec::DecodeArray(ByteArray& buffer,
	const std::vector<FieldType>& children){

	assert(!buffer.empty());

	//��������
	UnsignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	int size = (int)value->ToInt();
	delete value;

	//��ͷ
	ArrayValue* arrValue = new(std::nothrow) ArrayValue(children);
	assert(arrValue != nullptr);
	const std::vector<FieldType>& cols = arrValue->GetColTypes();

	for (int i=0; i<size; i++){//ѭ��ÿ��
		ArrayRowValue* row = arrValue->AddRow();
		if (row == nullptr){
			continue;
		}

		for (size_t j=0; j<cols.size(); j++){ //ѭ��ÿ��
			FieldType type = cols[j];
			IFieldValue* value = MessageCodecUtil::DecodeFieldValue(type, buffer);
			if (value != nullptr){
				row->AppendValue(value);
				delete value;
			}
		}
	}

	return arrValue;
}

KVArrayValue* MessageBodyUpdateCodec::DecodeKVArray(ByteArray& buffer, 
	const std::map<int, FieldType>& kvchildren){

	assert(!buffer.empty());
	KVArrayValue* arrValue = new(std::nothrow) KVArrayValue(kvchildren);
	assert(arrValue != nullptr);

	//��������
	UnsignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	int size = (int)value->ToInt();
	delete value;

	//����KV����������ʼ��ʾ0x0D
	Byte byt = buffer[0];
	buffer.erase(buffer.begin());

	for (int i=0; i<size; i++){ //ѭ��ÿ��
		KVArrayRowValue* row = arrValue->AddRow();
		if (row == nullptr){
			continue;
		}

		for (int fid : arrValue->GetColAllFids()){//��������
			byt = buffer[0];
			buffer.erase(buffer.begin());

			//�����Ƿ���ڵ�������Ϣ
			ByteArray valueTypeOpt = MessageBodyFullCodec::GetFieldInfoByte((Byte)byt);
			if (valueTypeOpt.empty() || valueTypeOpt[0] != 1) {// ����ֵ�Ŷ�ȡ
				continue;
			}

			FieldType type = arrValue->GetColType(fid);
			IFieldValue* value = MessageCodecUtil::GetValue(type, FieldDesc::NORMAL, buffer);
			if (value != nullptr){
				row->SetValue(fid, value);
				delete value;
			}
		}
	}

	return arrValue;
}

}
}