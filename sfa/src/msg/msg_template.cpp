#include "msg_template.h"
#include <assert.h>
#include <sfa/msg/value_array.h>
#include <sfa/msg/value_kv_array.h>

namespace sfa {
namespace msg {

MessageTemplate::~MessageTemplate(){
	for (size_t i=0; i<lstField_.size(); i++){
		delete lstField_[i];
	}
	lstField_.clear();
}

int MessageTemplate::GetVersion(void) const{
	return version_;
}

void MessageTemplate::SetVersion(int version){
	version_ = version;
}

void MessageTemplate::AddField(int fid, FieldType type){
	lstField_.push_back(new FieldIdAndType(fid, type));
}

void MessageTemplate::AddField(int fid, FieldType type,
	const std::vector<FieldType>& children){
	lstField_.push_back(new FieldIdAndType(fid, type, children));
}

void MessageTemplate::AddField(int fid, FieldType type,
	const std::vector<FieldType>& children,
	const std::map<int, FieldType>& kvChildren){
	lstField_.push_back(new FieldIdAndType(fid, type, children, kvChildren));
}

int MessageTemplate::GetFieldIndexByFID(int fid) const{
	for (size_t i=0; i<lstField_.size(); i++){
		FieldIdAndType *v = lstField_[i];
		if (v!=nullptr && v->GetFid()==fid)
			return i;
	}
	return -1;
}

FieldIdAndType* MessageTemplate::GetFieldByIndex(int idx) const{
	if (idx >= 0 && idx < (int)lstField_.size()){
		return lstField_[idx];
	}
	return nullptr;
}

std::vector<FieldIdAndType*> MessageTemplate::GetAllFieldType(void) const{
	return lstField_;
}

MessageTemplate* MessageTemplate::ToTemplate(const IMessageBody* body){
	assert(body != nullptr);
	return ToTemplate(body, 0);
}

MessageTemplate* MessageTemplate::ToTemplate(const IMessageBody* body, int version){
	assert(body != nullptr);

	MessageTemplate *temp = new(std::nothrow) MessageTemplate();
	assert(temp != nullptr);
	temp->SetVersion(version);

	for (int fid : body->GetAllFieldId()){
		IFieldValue *value = body->GetField(fid);
		if (value == nullptr){
			continue;
		}

		FieldType type = value->GetFieldType();
		if (type == FieldType::ARRAY){ //数组模板
			temp->AddField(fid, type, ((ArrayValue*)value)->GetColTypes());
		}
		else if (type == FieldType::KV_ARRAY){ //KV数组模板
			temp->AddField(fid, type, std::vector<FieldType>(), ((KVArrayValue*)value)->GetCols());
		}
		else{ //普通模板
			temp->AddField(fid, type);
		}
	}

	return temp;
}

}
}