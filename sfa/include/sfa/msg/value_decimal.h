#pragma once
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API DecimalValue : public IFieldValue{
public:
	explicit DecimalValue(FieldDesc desc);
	explicit DecimalValue(double value);
	DecimalValue(double mantissa, int exponent);
	
	double ToDouble(void);
	double GetMantissa(void) const;
	int GetExponent(void) const;

public:
	virtual FieldType GetFieldType(void) const override;
	virtual FieldDesc GetFieldDesc(void) const override;

	virtual int64_t ToInt(void) override;
	virtual uint64_t ToUInt(void) override;
	virtual std::string ToString(void) override;
	virtual ByteArray ToBytes(void) override;

	virtual IFieldValue* Clone(void) override;

private:
	double mantissa_ = 0;	//Î²Êý
	int exponent_ = 0;	//Ö¸Êý
	FieldDesc desc_ = FieldDesc::NORMAL;
};

}
}