#pragma once
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API AsciiValue : public IFieldValue{
public:
	explicit AsciiValue(FieldDesc desc);
	explicit AsciiValue(std::string value);

public:
	virtual FieldType GetFieldType(void) const override;
	virtual FieldDesc GetFieldDesc(void) const override;

	virtual int64_t ToInt(void) override;
	virtual uint64_t ToUInt(void) override;
	virtual std::string ToString(void) override;
	virtual ByteArray ToBytes(void) override;

	virtual IFieldValue* Clone(void) override;

private:
	std::string value_;
	FieldDesc desc_ = FieldDesc::NORMAL;
};

}
}