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

	//查找变化的索引
	std::vector<int> lstIdx;
	std::vector<int> fids;
	for (int fid : body->GetAllFieldId()){
		int idx = msgTemplate->GetFieldIndexByFID(fid);
		if (idx == -1){
			return ByteArray();
		}
		else if (body->GetField(fid)!=nullptr
			&& body->GetField(fid)->GetFieldDesc()==FieldDesc::NORMAL){//只有正常值才传递
			lstIdx.push_back(idx);
			fids.push_back(fid);
		}
	}

	ByteArray btBuffer;

	//根据变化生成存在图
	ByteArray btPMAP = EncodePMAP(lstIdx);
	if (btPMAP.empty()) {
		return ByteArray();
	}
	btBuffer.insert(btBuffer.end(), btPMAP.begin(), btPMAP.end());

	//生成内容
	for (int fid : fids){
		IFieldValue* value = body->GetField(fid);
		if (value == nullptr){
			continue;
		}

		ByteArray btValue;
		FieldType fieldType = value->GetFieldType();
		FieldDesc fieldDesc = value->GetFieldDesc();

		if (fieldType == FieldType::ARRAY){ //数组单独处理
			btValue = EncodeArray((ArrayValue*)value);
		}
		else if (fieldType == FieldType::KV_ARRAY){ //KV数组单独处理
			btValue = EncodeKVArray((KVArrayValue*)value);
		}
		else{
			//字段内容：只有字段有值才转换，不需要转换占位符
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

	//计算需要的字节数(每字节存7位)
	int maxIdx = lstIdx[lstIdx.size() - 1] + 1; //最大的索引号，索引从0开始，占用位数需要+1
	int len = maxIdx / 7;
	if (maxIdx % 7 != 0){ //不能被7整除
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

	//生成数组数量
	UnsignedIntegerValue tmpRowSize(arrValue->GetRows().size());
	UnsignedIntegerCodec codec;
	ByteArray btSize = codec.Encode(&tmpRowSize);
	btBuffer.insert(btBuffer.end(), btSize.begin(), btSize.end());

	//生成行数据
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

	//生成数组数量
	UnsignedIntegerValue tmpRowsSize(arrValue->GetRows().size());
	UnsignedIntegerCodec codec;
	ByteArray btSize = codec.Encode(&tmpRowsSize);
	btBuffer.insert(btBuffer.end(), btSize.begin(), btSize.end());

	//生成行数据
	for (KVArrayRowValue* row : arrValue->GetRows()){
		if (row == nullptr){
			continue;
		}

		//新行开始
		Byte newRow = MessageCodecUtil::GenFieldInfo(FIELDTYPE_KVARRAY_NEWROW,
			FieldDesc::RESERVED); 
		btBuffer.push_back(newRow);

		for (int fid : arrValue->GetColAllFids()){
			IFieldValue* value = row->GetValue(fid);
			if (value == nullptr) {// 此FID没有数据
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

	//根据存在图找到变化索引
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

	//数组行数
	UnsignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	int size = (int)value->ToInt();
	delete value;

	//表头
	ArrayValue* arrValue = new(std::nothrow) ArrayValue(children);
	assert(arrValue != nullptr);
	const std::vector<FieldType>& cols = arrValue->GetColTypes();

	for (int i=0; i<size; i++){//循环每行
		ArrayRowValue* row = arrValue->AddRow();
		if (row == nullptr){
			continue;
		}

		for (size_t j=0; j<cols.size(); j++){ //循环每列
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

	//数组行数
	UnsignedIntegerCodec codec;
	IFieldValue* value = codec.Decode(buffer);
	int size = (int)value->ToInt();
	delete value;

	//跳过KV数组增量开始标示0x0D
	Byte byt = buffer[0];
	buffer.erase(buffer.begin());

	for (int i=0; i<size; i++){ //循环每行
		KVArrayRowValue* row = arrValue->AddRow();
		if (row == nullptr){
			continue;
		}

		for (int fid : arrValue->GetColAllFids()){//解析内容
			byt = buffer[0];
			buffer.erase(buffer.begin());

			//数据是否存在的描述信息
			ByteArray valueTypeOpt = MessageBodyFullCodec::GetFieldInfoByte((Byte)byt);
			if (valueTypeOpt.empty() || valueTypeOpt[0] != 1) {// 存在值才读取
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