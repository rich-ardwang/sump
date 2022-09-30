#pragma once 
#include <sfa/msg/msg_def.h>
#include <sfa/msg/msg_header.h>
#include <sfa/msg/msg_body.h>

namespace sfa{
namespace msg{

class SFA_API IMessage {
public:
	virtual void SetHeader(_sfa_in_ IMessageHeader* header) = 0;
	virtual void SetBody(_sfa_in_ IMessageBody* body) = 0;

	virtual IMessageHeader* GetHeader(void) const = 0;
	virtual IMessageBody* GetBody(void) const = 0;

	virtual IMessage* Clone(void) = 0;
	virtual std::string ToString(void) = 0;
};

SFA_API IMessage* NewMessage(void);
SFA_API void DeleteMessage(IMessage** message);

}
}