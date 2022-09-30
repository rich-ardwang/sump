#pragma once
#include <vector>
#include <map>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API KVArrayRowValue{
public:
	explicit KVArrayRowValue(const std::map<int, FieldType>& cols);
	virtual ~KVArrayRowValue();

	KVArrayRowValue(const KVArrayRowValue& value);
	KVArrayRowValue& operator=(const KVArrayRowValue& value);
	
public:
	bool SetValue(int fid, const IFieldValue* value);
	IFieldValue* GetValue(int fid);
	void SetValues(const std::map<int, IFieldValue*>& values);
	std::vector<int> GetAllFids(void) const;

	void Clear(void);
	size_t Size(void) const;
	KVArrayRowValue* Clone(void);

private:
	std::map<int, FieldType> cols_;
	std::map<int, IFieldValue*> values_;
};

}
}