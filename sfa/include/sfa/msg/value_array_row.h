#pragma once
#include <vector>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API ArrayRowValue{
public:
	explicit ArrayRowValue(const std::vector<FieldType>& cols);
	virtual ~ArrayRowValue();

	ArrayRowValue(const ArrayRowValue& value);
	ArrayRowValue& operator=(const ArrayRowValue& value);

public:
	bool AppendValue(const IFieldValue* value);
	void SetValue(const std::vector<IFieldValue*>& values);
	const std::vector<IFieldValue*>& GetValues(void) const;

	void Clear(void);
	size_t Size(void) const;
	ArrayRowValue* Clone(void);

private:
	std::vector<FieldType> cols_;
	std::vector<IFieldValue*> values_;
};

}
}