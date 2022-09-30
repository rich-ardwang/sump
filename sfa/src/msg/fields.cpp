#include "fields.h"

namespace sfa {
namespace msg {

Fields::Fields(const Fields& fields){
	for (auto iter = fields.mapFields_.begin();
		iter != fields.mapFields_.end(); ++iter){
		mapFields_[iter->first] = iter->second->Clone();
	}
}

Fields::Fields(Fields&& fields){
	for (auto iter = fields.mapFields_.begin();
		iter != fields.mapFields_.end(); ++iter){
		mapFields_[iter->first] = iter->second;
		iter->second = nullptr;
	}
	fields.mapFields_.clear();
}

Fields& Fields::operator=(const Fields& fields){
	if (this != &fields){
		Clear();
		for (auto iter = fields.mapFields_.begin();
			iter != fields.mapFields_.end(); ++iter){
			mapFields_[iter->first] = iter->second->Clone();
		}
	}
	return *this;
}

Fields& Fields::operator=(Fields&& fields){
	if (this != &fields){
		for (auto iter=fields.mapFields_.begin();
			iter!=fields.mapFields_.end(); ++iter){
			mapFields_[iter->first] = iter->second;
			iter->second = nullptr;
		}
		fields.mapFields_.clear();
	}
	return *this;
}

Fields::~Fields(){
	Clear();
}

IFieldValue* Fields::Get(int fid) const{
	auto iter = mapFields_.find(fid);
	if (iter != mapFields_.end()){
		return iter->second;
	}
	return nullptr;
}

void Fields::Add(int fid, const IFieldValue *value){
	auto iter = mapFields_.find(fid);
	if (iter != mapFields_.end()){
		*iter->second = *value;
	}
	else {
		mapFields_[fid] = const_cast<IFieldValue*>(value)->Clone();
	}
}

void Fields::Del(int fid){
	auto iter = mapFields_.find(fid);
	if (iter != mapFields_.end()){
		delete iter->second;
		mapFields_.erase(iter);
	}
}

void Fields::Add(const std::map<int, IFieldValue*>& mapFields){
	Clear();
	for (auto iter = mapFields_.begin(); iter != mapFields_.end(); ++iter){
		mapFields_[iter->first] = iter->second->Clone();
	}
}

std::vector<int> Fields::GetAllFieldId(void) const{
	std::vector<int> fids;
	fids.reserve(mapFields_.size());

	for (auto iter=mapFields_.begin(); iter!=mapFields_.end(); ++iter){
		fids.push_back(iter->first);
	}
	return fids;
}

int Fields::Size(void) const{
	return mapFields_.size();
}

void Fields::Clear(void){
	for (auto iter=mapFields_.begin(); iter!=mapFields_.end(); ++iter){
		if (iter->second != nullptr){
			delete iter->second;
		}
	}
	mapFields_.clear();
}

}
}