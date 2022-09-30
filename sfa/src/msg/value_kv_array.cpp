#include <sfa/msg/value_kv_array.h>
#include <assert.h>

namespace sfa {
namespace msg {

KVArrayValue::KVArrayValue(){
}

KVArrayValue::KVArrayValue(const std::map<int, FieldType>& cols){
	cols_ = cols;
}

KVArrayValue::KVArrayValue(const KVArrayValue& value){
	cols_ = value.cols_;
	for (size_t i = 0; i < value.rows_.size(); i++){
		rows_.push_back(value.rows_[i]->Clone());
	}
}

KVArrayValue& KVArrayValue::operator=(const KVArrayValue& value){
	if (this != &value){
		clear();
		cols_ = value.cols_;
		for (size_t i = 0; i < value.rows_.size(); i++){
			rows_.push_back(value.rows_[i]->Clone());
		}
	}
	return *this;
}

FieldType KVArrayValue::GetFieldType(void) const{
	return FieldType::KV_ARRAY;
}

FieldDesc KVArrayValue::GetFieldDesc(void) const{
	return FieldDesc::RESERVED;
}

KVArrayValue::~KVArrayValue(){
	clear();
}

void KVArrayValue::clear(void){
	for (size_t i = 0; i < rows_.size(); i++){
		delete rows_[i];
		rows_[i] = nullptr;
	}
	rows_.clear();
}

void KVArrayValue::SetColType(int fid, FieldType type){
	cols_[fid] = type;
}

FieldType KVArrayValue::GetColType(int fid) const{
	auto iter = cols_.find(fid);
	if (iter != cols_.end()){
		return iter->second;
	}

	assert(0);
	return FieldType::INTEGER;
}

std::vector<int> KVArrayValue::GetColAllFids(void) const{
	std::vector<int> fids;
	for (auto iter = cols_.begin(); iter != cols_.end(); ++iter){
		fids.push_back(iter->first);
	}
	std::sort(fids.begin(), fids.end());
	return fids;
}

std::map<int, FieldType> KVArrayValue::GetCols(void) const{
	return cols_;
}

KVArrayRowValue* KVArrayValue::AddRow(void){
	KVArrayRowValue *row = new KVArrayRowValue(cols_);
	rows_.push_back(row);
	return row;
}

std::vector<KVArrayRowValue*> KVArrayValue::GetRows(void) const{
	return rows_;
}

IFieldValue* KVArrayValue::Clone(void){
	return new KVArrayValue(*this);
}

int64_t KVArrayValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t KVArrayValue::ToUInt(void) {
	assert(0);
	return 0;
}

ByteArray KVArrayValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

std::string KVArrayValue::ToString(void){
	std::string str;

	for (KVArrayRowValue *row : rows_){
		if (row == nullptr){
			continue;
		}

		str.append("[");
		for (int fid : row->GetAllFids()){
			str.append("{");
			str.append("Fid:");
			str.append(std::to_string(fid));
			str.append(",Value:");
			str.append(row->GetValue(fid)->ToString());
			str.append("}");
		}
		str.append("]");
	}

	return str;
}

}
}