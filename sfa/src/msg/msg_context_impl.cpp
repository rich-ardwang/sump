#include "msg_context_impl.h"
#include <assert.h>
#include "utils_other.h"

namespace sfa {
namespace msg {

IMessageContext* NewMessageContext(void) {
	return new(std::nothrow)MessageContext();
}

void DeleteMessageContext(IMessageContext** context) {
	utils::DeletePtr<IMessageContext>(context);
}

MessageContext::~MessageContext(){
	auto iter = templates_.begin();
	for (; iter!= templates_.end(); ++iter){
		delete iter->second;
	}
	templates_.clear();
}

void MessageContext::RegisterTemplate(const IMessage* message){
	assert(message != nullptr);
	if (message->GetHeader()==nullptr
		|| message->GetBody()==nullptr){
		return;
	}

	int templateId = message->GetHeader()->GetTemplateId();
	int version = message->GetHeader()->GetVersion();
	const IMessageBody* body = message->GetBody();
	RegisterTemplate(templateId, version, body);
}

void MessageContext::RegisterTemplate(int template_id, int version, const IMessageBody* body){
	assert(body != nullptr);
	auto iter = templates_.find(template_id);
	if (iter != templates_.end()){
		delete iter->second;
		templates_.erase(iter);
	}

	MessageTemplate *temp = MessageTemplate::ToTemplate(body, version);
	assert(temp != nullptr);
	templates_[template_id] = temp;
}

bool MessageContext::IsTemplateUpdate(const IMessage* message){
	assert(message != nullptr);
	if (message->GetHeader()==nullptr){
		return false;
	}

	int templateId = message->GetHeader()->GetTemplateId();
	int version = message->GetHeader()->GetVersion();
	return IsTemplateUpdate(templateId, version);
}

bool MessageContext::IsTemplateUpdate(int template_id, int version){
	MessageTemplate *msgTemplate = GetTemplate(template_id);
	if (msgTemplate==nullptr 
		|| msgTemplate->GetVersion()!=version){
		return true;
	}
	return false;
}

MessageTemplate* MessageContext::GetTemplate(int template_id){
	auto iter = templates_.find(template_id);
	if (iter != templates_.end()){
		return iter->second;
	}
	return nullptr;
}

}
}