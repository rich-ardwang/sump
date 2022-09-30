#pragma once
#include <sfa/msg/msg_context.h>
#include "msg_template.h"

namespace sfa {
namespace msg {

class MessageContext : public IMessageContext{
public:
	MessageContext() = default;
	virtual ~MessageContext();

	virtual void RegisterTemplate(const IMessage* message) override;
	virtual void RegisterTemplate(int template_id, int version, const IMessageBody* body) override;

	virtual bool IsTemplateUpdate(const IMessage* message) override;
	virtual bool IsTemplateUpdate(int template_id, int version) override;

	MessageTemplate* GetTemplate(int template_id);

private:
	std::map<int, MessageTemplate*> templates_;
};

}
}