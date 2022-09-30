#pragma once
#include <vector>
#include <sfa/msg/value_field.h>
#include <sfa/msg/value_array_row.h>

namespace sfa {
namespace msg {

class SFA_API ArrayValue : public IFieldValue{
public:
	ArrayValue();
	explicit ArrayValue(const std::vector<FieldType>& cols);
	virtual ~ArrayValue();

	ArrayValue(const ArrayValue& value);
	ArrayValue& operator=(const ArrayValue& value);

public:
	void AddColType(FieldType type);
	const std::vector<FieldType>& GetColTypes(void) const;
	FieldType GetColType(int idx);

	ArrayRowValue* AddRow(void);
	const std::vector<ArrayRowValue*>& GetRows(void) const;

public:
	virtual FieldType GetFieldType(void) const override;
	virtual FieldDesc GetFieldDesc(void) const override;

	virtual int64_t ToInt(void) override;
	virtual uint64_t ToUInt(void) override;
	virtual std::string ToString(void) override;
	virtual ByteArray ToBytes(void) override;

	virtual IFieldValue* Clone(void) override;

private:
	void Clear(void);

private:
	std::vector<FieldType> cols_;
	std::vector<ArrayRowValue*> rows_;
};

}
}