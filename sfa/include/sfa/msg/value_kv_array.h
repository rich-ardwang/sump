#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <sfa/msg/value_field.h>
#include <sfa/msg/value_kv_array_row.h>

namespace sfa {
namespace msg {

class SFA_API KVArrayValue : public IFieldValue{
public:
	KVArrayValue();
	explicit KVArrayValue(const std::map<int, FieldType>& cols);
	virtual ~KVArrayValue();

	KVArrayValue(const KVArrayValue& value);
	KVArrayValue& operator=(const KVArrayValue& value);

public:
	void SetColType(int fid, FieldType type);
	FieldType GetColType(int fid) const;

	std::vector<int> GetColAllFids(void) const;
	std::map<int, FieldType> GetCols(void) const;

	KVArrayRowValue* AddRow(void);
	std::vector<KVArrayRowValue*> GetRows(void) const;

public:
	virtual FieldType GetFieldType(void) const override;
	virtual FieldDesc GetFieldDesc(void) const override;

	virtual int64_t ToInt(void) override;
	virtual uint64_t ToUInt(void) override;
	virtual ByteArray ToBytes(void) override;
	virtual std::string ToString(void) override;

	virtual IFieldValue* Clone(void) override;

private:
	void clear(void);

private:
	std::map<int, FieldType> cols_;
	std::vector<KVArrayRowValue*> rows_;
};

}
}