#pragma once
#include <time.h>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API TimeValue : public IFieldValue{
public:
	explicit TimeValue(FieldDesc desc);
	explicit TimeValue(time_t value);
	TimeValue(int hour, int min, int sec);

	time_t GetValue(void) const;

public:
	virtual FieldType GetFieldType(void) const override;
	virtual FieldDesc GetFieldDesc(void) const override;

	virtual int64_t ToInt(void) override;
	virtual uint64_t ToUInt(void) override;
	virtual ByteArray ToBytes(void) override;
	virtual std::string ToString(void) override;

	virtual IFieldValue* Clone(void) override;

private:
	time_t value_;
	FieldDesc desc_ = FieldDesc::NORMAL;
};

}
}