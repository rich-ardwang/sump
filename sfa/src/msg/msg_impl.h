#pragma once 
#include <sfa/msg/msg.h>

namespace sfa {
namespace msg {

/**
*
* ��Ϣ����ɣ���Ϣ = ��Ϣͷ + ��Ϣ��
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
	IMessageHeader* header_ = nullptr; //��Ϣͷ
	IMessageBody* body_ = nullptr; //��Ϣ��
};

}
}