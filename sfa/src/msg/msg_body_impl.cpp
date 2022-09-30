#include "msg_body_impl.h"
#include <algorithm>
#include <assert.h>

namespace sfa {
namespace msg {

void MessageBody::AddField(int fid, const IFieldValue* value) {
	assert(value != nullptr);
	fields_.Add(fid, value);
}

IFieldValue* MessageBody::GetField(int fid) const {
	return fields_.Get(fid);
}

void MessageBody::DelField(int fid) {
	fields_.Del(fid);
}

int MessageBody::GetFieldSize(void) const {
	return fields_.Size();
}

std::vector<int> MessageBody::GetAllFieldId(void) const {
	std::vector<int> fids = fields_.GetAllFieldId();
	//排序保证大小顺序，方便模板建立存在图
	std::sort(fids.begin(), fids.end());
	return fids;
}

IMessageBody* MessageBody::Clone(void) {
	return new MessageBody(*this);
}

std::string MessageBody::ToString(void) {
	std::string str;
	for (int fid : GetAllFieldId()) {
		str.append("[");
		str.append(std::to_string(fid));
		str.append("]");

		IFieldValue* value = GetField(fid);
		if (value != nullptr) {
			if (value->GetFieldType() == FieldType::ARRAY) {
				str.append(ShowArray((ArrayValue*)value));
			}
			else if (value->GetFieldType() == FieldType::KV_ARRAY) {
				str.append(ShowKVArray((KVArrayValue*)value));
			}
			else {
				str.append(ShowField(value));
			}
			str.append(";\r\n");
		}
	}

	return str;
}

std::string MessageBody::ShowArray(ArrayValue* value) {
	std::string str;

	str.append("{");
	str.append(GetFieldTypeStr(value->GetFieldType()));
	str.append("[");
	for (FieldType type : value->GetColTypes()) {
		str.append(GetFieldTypeStr(type));
		str.append("|");
	}
	str.append("]");

	for (ArrayRowValue* row : value->GetRows()) {
		if (row == nullptr) {
			continue;
		}
		str.append("[");
		for (IFieldValue* rowval : row->GetValues()) {
			if (rowval == nullptr) {
				continue;
			}
			str.append(rowval->ToString());
			str.append("|");
		}

		str.append("]\r\n");
	}

	str.append("}");

	return str;
}

std::string MessageBody::ShowKVArray(KVArrayValue* value) {
	std::string str;

	str.append("(");
	str.append(GetFieldTypeStr(value->GetFieldType()));
	str.append(")");
	str.append(":[");

	auto cols = value->GetCols();
	for (auto iter = cols.begin(); iter != cols.end(); ++iter) {
		str.append(std::to_string(iter->first));
		str.append("(");
		str.append(GetFieldTypeStr(iter->second));
		str.append(")");
		str.append("|");
	}
	str.append("]");

	for (KVArrayRowValue* row : value->GetRows()) {
		if (row == nullptr) {
			continue;
		}
		str.append("[");
		for (int fid : row->GetAllFids()) {
			str.append(std::to_string(fid));
			str.append("-");
			str.append(row->GetValue(fid)->ToString());
			str.append("|");
		}
		str.append("]\r\n");
	}

	return str;
}

std::string MessageBody::ShowField(IFieldValue* value) {
	assert(value != nullptr);
	std::string str;

	str.append("(");
	str.append(GetFieldTypeStr(value->GetFieldType()));
	if (value->GetFieldDesc() != FieldDesc::NORMAL) {
		str.append("-");
		str.append(std::to_string((int)value->GetFieldDesc()));
	}
	str.append(")");
	if (value->GetFieldDesc() == FieldDesc::NORMAL) {
		str.append(":[");
		str.append(value->ToString());
		str.append("]");
	}

	return str;
}

std::string MessageBody::GetFieldTypeStr(FieldType type) {
	std::string fieldType = "";

	switch (type) {
	case FieldType::INTEGER:
		fieldType = "INTEGER";
		break;
	case FieldType::UNSIGNED_INTEGER:
		fieldType = "UNSIGNED_INTEGER";
		break;
	case FieldType::DECIMAL:
		fieldType = "DECIMAL";
		break;
	case FieldType::ASCII:
		fieldType = "ASCII";
		break;
	case FieldType::UNIC0DE:
		fieldType = "UNIC0DE";
		break;
	case FieldType::BYTE:
		fieldType = "BYTE";
		break;
	case FieldType::DATE:
		fieldType = "DATE";
		break;
	case FieldType::TIME:
		fieldType = "TIME";
		break;
	case FieldType::ARRAY:
		fieldType = "ARRAY";
		break;
	case FieldType::KV_ARRAY:
		fieldType = "KV_ARRAY";
		break;
	default:
		assert(0);
		break;
	}

	return fieldType;
}

}
}

