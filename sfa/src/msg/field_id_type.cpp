#include "field_id_type.h"

namespace sfa {
namespace msg {

FieldIdAndType::FieldIdAndType(int fid, FieldType type){
	fid_ = fid;
	type_ = type;
}

FieldIdAndType::FieldIdAndType(int fid, FieldType type,
	const std::vector<FieldType>& children)
	: FieldIdAndType(fid, type){
	children_ = children;
}

FieldIdAndType::FieldIdAndType(int fid, FieldType type,
		const std::vector<FieldType>& children,
		const std::map<int, FieldType>& kvChildren)
		: FieldIdAndType(fid, type){
	children_ = children;
	kvChildren_ = kvChildren;
}

const std::vector<FieldType>& FieldIdAndType::GetChildren(void) const{
	return children_;
}

void FieldIdAndType::SetChildren(const std::vector<FieldType>& children){
	children_ = children;
}

const std::map<int, FieldType>& FieldIdAndType::GetKVChildren(void) const{
	return kvChildren_;
}

void FieldIdAndType::SetKVChildren(const std::map<int, FieldType>& kvChildren){
	kvChildren_ = kvChildren;
}

int FieldIdAndType::GetFid(void) const{
	return fid_;
}

void FieldIdAndType::SetFid(int fid){
	fid_ = fid;
}

FieldType FieldIdAndType::GetType(void) const{
	return type_;
}

void FieldIdAndType::SetType(FieldType type){
	type_ = type;
}

}
}
