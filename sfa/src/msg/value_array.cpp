#include <sfa/msg/value_array.h>
#include <assert.h>

namespace sfa {
namespace msg {

ArrayValue::ArrayValue(){
}

ArrayValue::ArrayValue(const std::vector<FieldType>& cols){
	cols_ = cols;
}

ArrayValue::ArrayValue(const ArrayValue& value){
	cols_ = value.cols_;
	for (size_t i=0; i<value.rows_.size(); i++){
		rows_.push_back(value.rows_[i]->Clone());
	}
}

ArrayValue& ArrayValue::operator=(const ArrayValue& value){
	if (this != &value){
		Clear();
		cols_ = value.cols_;
		for (size_t i=0; i<value.rows_.size(); i++){
			rows_.push_back(value.rows_[i]->Clone());
		}
	}
	return *this;
}

ArrayValue::~ArrayValue(){
	Clear();
}

void ArrayValue::Clear(void){
	for (size_t i=0; i<rows_.size(); i++){
		delete rows_[i];
		rows_[i] = nullptr;
	}
	rows_.clear();
}

void ArrayValue::AddColType(FieldType type){
	cols_.push_back(type);
}

const std::vector<FieldType>& ArrayValue::GetColTypes(void) const{
	return cols_;
}

FieldType ArrayValue::GetColType(int idx){
	assert(idx>=0 && idx<(int)cols_.size());
	return cols_[idx];
}

ArrayRowValue* ArrayValue::AddRow(void){
	ArrayRowValue *row = new ArrayRowValue(cols_);
	rows_.push_back(row);
	return row;
}

const std::vector<ArrayRowValue*>& ArrayValue::GetRows(void) const{
	return rows_;
}

IFieldValue* ArrayValue::Clone(void){
	return new ArrayValue(*this);
}

FieldType ArrayValue::GetFieldType(void) const{
	return FieldType::ARRAY;
}

FieldDesc ArrayValue::GetFieldDesc(void) const{
	return FieldDesc::RESERVED;
}

int64_t ArrayValue::ToInt(void){
	assert(0);
	return 0;
}

uint64_t ArrayValue::ToUInt(void) {
	assert(0);
	return 0;
}

std::string ArrayValue::ToString(void){
	std::string str;

	for (ArrayRowValue* row : rows_){
		if (row == nullptr){
			continue;
		}

		str.append("[");
		for (IFieldValue* value : row->GetValues()){
			str.append("{");
			str.append("Value:");
			str.append(value->ToString());
			str.append("}");
		}
		str.append("]");
	}

	return str;
}

ByteArray ArrayValue::ToBytes(void){
	assert(0);
	return ByteArray();
}

}
}