#pragma once
#include <vector>
#include <sfa/msg/msg_def.h>
#include <sfa/msg/value_field.h>

namespace sfa {
namespace msg {

class SFA_API IMessageHeader {
public:
	enum MessageType {
		FULL,		 //全量
		INCREMENTAL, //增量
		TEMPLATE,	 //模板
		UNKNOWN		 //未知，初始化用
	};

	virtual MessageType GetMessageType(void) const = 0;
	virtual void SetMessageType(_sfa_in_ MessageType msg_type) = 0;

	virtual int GetTemplateId(void) const = 0;
	virtual void SetTemplateId(int template_id) = 0;

	virtual int GetVersion(void) const = 0;
	virtual void SetVersion(_sfa_in_ int version) = 0;

	virtual IFieldValue* GetExtraField(_sfa_in_ int fid) const = 0;
	virtual void AddExtraField(_sfa_in_ int fid, _sfa_in_ const IFieldValue* value) = 0;
	virtual void DelExtraField(_sfa_in_ int fid) = 0;

	virtual std::vector<int> GetAllExtraFieldId(void) const = 0;
	virtual int GetExtraFieldSize(void) const = 0;

	virtual std::string ToString(void) = 0;
	virtual IMessageHeader* Clone(void) = 0;
};

}
}