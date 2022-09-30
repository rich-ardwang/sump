#pragma once
#include <time.h>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API DateValue :public IFieldValue {
public:
	explicit DateValue(FieldDesc desc);
	explicit DateValue(time_t value);
	DateValue(int year, int month, int day);

	time_t GetValue(void) const;

public:
	virtual FieldType GetFieldType(void) const override;
	virtual FieldDesc GetFieldDesc(void) const override;

	virtual int64_t ToInt(void) override;
	virtual uint64_t ToUInt(void) override;
	virtual std::string ToString(void) override;
	virtual ByteArray ToBytes(void) override;

	virtual IFieldValue* Clone(void) override;

private:
	time_t value_;
	FieldDesc desc_ = FieldDesc::NORMAL;
};

}
}