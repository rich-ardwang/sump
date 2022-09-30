#include "msg_impl.h"
#include <assert.h>
#include "msg_header_impl.h"
#include "msg_body_impl.h"
#include "utils_other.h"

namespace sfa {
namespace msg {

IMessage* NewMessage(void) {
	return new(std::nothrow) Message();
}

void DeleteMessage(IMessage** message) {
	utils::DeletePtr<IMessage>(message);
}

Message::Message()
	: header_(new MessageHeader)
	, body_(new MessageBody) {
}

Message::Message(const Message& msg) {
	header_ = msg.header_->Clone();
	body_ = msg.body_->Clone();
}

Message::Message(Message&& msg) {
	header_ = msg.header_;
	msg.header_ = nullptr;
	body_ = msg.body_;
	msg.body_ = nullptr;
}

Message& Message::operator=(const Message& msg) {
	if (this != &msg) {
		Clear();
		header_ = msg.header_->Clone();
		body_ = msg.body_->Clone();
	}
	return *this;
}

Message& Message::operator=(Message&& msg) {
	if (this != &msg) {
		header_ = msg.header_;
		msg.header_ = nullptr;
		body_ = msg.body_;
		msg.body_ = nullptr;
	}
	return *this;
}

Message::~Message() {
	Clear();
}

void Message::Clear(void) {
	if (header_ != nullptr) {
		delete header_;
		header_ = nullptr;
	}

	if (body_ != nullptr) {
		delete body_;
		body_ = nullptr;
	}
}

void Message::SetHeader(IMessageHeader* header) {
	assert(header != nullptr);
	if (header != nullptr) {
		*header_ = *header;
	}
}

IMessageHeader* Message::GetHeader(void) const {
	return header_;
}

void Message::SetBody(IMessageBody* body) {
	assert(body != nullptr);
	if (body != nullptr) {
		*body_ = *body;
	}
}

IMessageBody* Message::GetBody(void) const {
	return body_;
}

IMessage* Message::Clone(void) {
	return new Message(*this);
}

std::string Message::ToString(void) {
	assert(header_ != nullptr);
	assert(body_ != nullptr);

	return "{\r\nheader:\r\n" + header_->ToString()
		+ ", \r\nbody:\r\n" + body_->ToString() + "\r\n}";
}

}
}