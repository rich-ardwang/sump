#pragma once 
#include <sfa/msg/msg.h>

namespace sfa {
namespace msg {

/**
*
* 消息，组成：消息 = 消息头 + 消息体
*
*/
class Message : public IMessage {
public:
	Message();
	Message(const Message& msg);
	Message(Message&& msg);
	Message& operator=(const Message& msg);
	Message& operator=(Message&& msg);
	virtual ~Message();

public:
	void SetHeader(IMessageHeader* header) override;
	void SetBody(IMessageBody* body) override;

	IMessageHeader* GetHeader(void) const override;
	IMessageBody* GetBody(void) const override;

	IMessage* Clone(void) override;
	std::string ToString(void) override;

private:
	void Clear(void);

private:
	IMessageHeader* header_ = nullptr; //消息头
	IMessageBody* body_ = nullptr; //消息体
};

}
}