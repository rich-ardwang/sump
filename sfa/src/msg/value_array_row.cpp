#include <sfa/msg/value_array_row.h>
#include <assert.h>

namespace sfa {
namespace msg {

ArrayRowValue::ArrayRowValue(const std::vector<FieldType>& cols){
	cols_ = cols;
}

ArrayRowValue::ArrayRowValue(const ArrayRowValue& value){
	cols_ = value.cols_;
	for (size_t i=0; i<value.values_.size(); i++){
		values_.push_back(value.values_[i]->Clone());
	}
}

ArrayRowValue& ArrayRowValue::operator=(const ArrayRowValue& value){
	if (this != &value){
		Clear();
		cols_ = value.cols_;
		for (size_t i=0; i<value.values_.size(); i++){
			values_.push_back(value.values_[i]->Clone());
		}
	}
	return *this;
}

ArrayRowValue::~ArrayRowValue(){
	Clear();
}

bool ArrayRowValue::AppendValue(const IFieldValue* value){
	assert(value != nullptr);
	if (values_.size() >= cols_.size()){//列数超过列类型数
		return false;
	}

	if (cols_[values_.size()] != value->GetFieldType()){//行类型与列类型不一样
		return false;
	}

	values_.push_back(const_cast<IFieldValue*>(value)->Clone());
	return true;
}

void ArrayRowValue::SetValue(const std::vector<IFieldValue*>& values){
	for (size_t i=0; i<values.size(); i++){
		values_.push_back(values[i]->Clone());
	}
}

const std::vector<IFieldValue*>& ArrayRowValue::GetValues(void) const{
	return values_;
}

void ArrayRowValue::Clear(void){
	for (size_t i=0; i<values_.size(); i++){
		delete values_[i];
		values_[i] = nullptr;
	}
	values_.clear();
	cols_.clear();
}

size_t ArrayRowValue::Size(void) const{
	return values_.size();
}

ArrayRowValue* ArrayRowValue::Clone(void){
	return new ArrayRowValue(*this);
}

}
}