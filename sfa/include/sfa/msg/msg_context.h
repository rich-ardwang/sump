#pragma once
#include <sfa/msg/msg_def.h>
#include <sfa/msg/msg.h>

namespace sfa {
namespace msg {

class SFA_API IMessageContext {
public:
	virtual void RegisterTemplate(_sfa_in_ const IMessage* message) = 0;
	virtual void RegisterTemplate(_sfa_in_ int template_id, _sfa_in_ int version, _sfa_in_ const IMessageBody* body) = 0;

	virtual bool IsTemplateUpdate(_sfa_in_ const IMessage* message) = 0;
	virtual bool IsTemplateUpdate(_sfa_in_ int template_id, _sfa_in_ int version) = 0;
};

SFA_API IMessageContext* NewMessageContext(void);
SFA_API void DeleteMessageContext(IMessageContext** context);

}
}