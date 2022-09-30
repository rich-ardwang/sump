#pragma once
#include <sfa/msg/msg_def.h>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API IMessageBody {
public:
	virtual void AddField(_sfa_in_ int fid, _sfa_in_ const IFieldValue* value) = 0;
	virtual IFieldValue* GetField(_sfa_in_ int fid) const = 0;
	virtual void DelField(_sfa_in_ int fid) = 0;
	virtual std::vector<int> GetAllFieldId(void) const = 0;
	virtual int GetFieldSize(void) const = 0;

	virtual std::string ToString(void) = 0;
	virtual IMessageBody* Clone(void) = 0;
};

}
}