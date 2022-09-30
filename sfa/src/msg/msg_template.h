#pragma once
#include <vector>
#include <sfa/msg/value_field.h>
#include <sfa/msg/msg_body.h>
#include "field_id_type.h"

namespace sfa {
namespace msg {

class MessageTemplate{
public:
	MessageTemplate() = default;
	virtual ~MessageTemplate();

public:
	int GetVersion(void) const;
	void SetVersion(int version);

	void AddField(int fid, FieldType type);
	void AddField(int fid, FieldType type, const std::vector<FieldType>& children);
	void AddField(int fid, FieldType type, const std::vector<FieldType>& children, 
		const std::map<int, FieldType>& kvChildren);

	int GetFieldIndexByFID(int fid) const;
	FieldIdAndType* GetFieldByIndex(int idx) const;
	std::vector<FieldIdAndType*> GetAllFieldType(void) const;

public:
	static MessageTemplate* ToTemplate(const IMessageBody* body);
	static MessageTemplate* ToTemplate(const IMessageBody* body, int version);

private:
	int version_ = 0;
	std::vector<FieldIdAndType*> lstField_;
};

}
}