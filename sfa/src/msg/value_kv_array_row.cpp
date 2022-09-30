#include <sfa/msg/value_kv_array_row.h>
#include <assert.h>

namespace sfa {
namespace msg {

KVArrayRowValue::KVArrayRowValue(const std::map<int, FieldType>& cols){
	cols_ = cols;
}

KVArrayRowValue::KVArrayRowValue(const KVArrayRowValue& value){
	cols_ = value.cols_;
	for (auto iter = value.values_.begin();
		iter != value.values_.end(); ++iter){
		values_[iter->first] = iter->second->Clone();
	}
}

KVArrayRowValue& KVArrayRowValue::operator=(const KVArrayRowValue& value){
	if (this != &value){
		Clear();
		cols_ = value.cols_;
		for (auto iter = value.values_.begin();
			iter != value.values_.end(); ++iter){
			values_[iter->first] = iter->second->Clone();
		}
	}
	return *this;
}

KVArrayRowValue::~KVArrayRowValue(){
	Clear();
}

bool KVArrayRowValue::SetValue(int fid, const IFieldValue* value){
	assert(value != nullptr);

	auto iter = cols_.find(fid);
	if (iter == cols_.end()){//行FID没在列中定义
		return false;
	}

	if (iter->second != value->GetFieldType()){//行类型与列类型不一样
		return false;
	}

	values_[fid] = const_cast<IFieldValue*>(value)->Clone();
	return true;
}

IFieldValue* KVArrayRowValue::GetValue(int fid){
	auto iter = values_.find(fid);
	if (iter != values_.end()){
		return iter->second;
	}
	return nullptr;
}

void KVArrayRowValue::SetValues(const std::map<int, IFieldValue*>& values){
	for (auto iter = values.begin(); iter != values.end(); ++iter){
		values_[iter->first] = iter->second->Clone();
	}
}

std::vector<int> KVArrayRowValue::GetAllFids(void) const{
	std::vector<int> fids;
	for (auto iter = values_.begin();
		iter != values_.end(); ++iter){
		fids.push_back(iter->first);
	}
	return fids;
}

void KVArrayRowValue::Clear(void){
	for (auto iter = values_.begin(); 
		iter != values_.end(); ++iter){
		delete iter->second;
	}
	values_.clear();
	cols_.clear();
}

size_t KVArrayRowValue::Size(void) const{
	return values_.size();
}

KVArrayRowValue* KVArrayRowValue::Clone(void){
	return new KVArrayRowValue(*this);
}

}
}