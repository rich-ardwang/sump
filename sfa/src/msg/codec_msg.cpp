#include "codec_msg.h"
#include <assert.h>
#include "codec_msg_header.h"
#include "codec_msg_body_full.h"
#include "codec_msg_body_update.h"
#include "msg_impl.h"

namespace sfa {
namespace msg {

ByteArray MessageCodec::Encode(const IMessage* message, IMessageContext* context){
	assert(message != nullptr);
	assert(message->GetHeader() != nullptr);
	assert(message->GetBody() != nullptr);
	assert(context != nullptr);

	//消息头编码
	ByteArray btHeader = MessageHeaderCodec::Encode(message->GetHeader());

	//消息体编码
	ByteArray btBody;
	if (message->GetHeader()->GetMessageType() == IMessageHeader::FULL
		|| message->GetHeader()->GetMessageType() == IMessageHeader::INCREMENTAL){
		btBody = MessageBodyFullCodec::Encode(message->GetBody());
	}
	else if (message->GetHeader()->GetMessageType() == IMessageHeader::TEMPLATE){
		int templateId = message->GetHeader()->GetTemplateId();
		btBody = MessageBodyUpdateCodec::Encode(message->GetBody(), templateId, context);
	}
	else{
		assert(0);
	}

	ByteArray btBuffer;
	btBuffer.insert(btBuffer.end(), btHeader.begin(), btHeader.end());
	btBuffer.insert(btBuffer.end(), btBody.begin(), btBody.end());
	return btBuffer;
}

bool MessageCodec::Decode(ByteArray& buffer, IMessageContext* context, IMessage*& message){
	assert(!buffer.empty());
	assert(context != nullptr);
	assert(message == nullptr);
	if (message != nullptr) {
		return false;
	}

	IMessage* tmp_msg = new (std::nothrow)Message();
	if (tmp_msg == nullptr) {
		return false;
	}

	IMessageHeader* header = tmp_msg->GetHeader();
	bool decode_header_succeed = MessageHeaderCodec::Decode(buffer, header);
	if (!decode_header_succeed) {
		delete tmp_msg;
		return false;
	}

	bool decode_body_succeed = false;
	IMessageBody* body = tmp_msg->GetBody();
	if (header->GetMessageType() == IMessageHeader::FULL
		|| header->GetMessageType() == IMessageHeader::INCREMENTAL){
		decode_body_succeed = MessageBodyFullCodec::Decode(buffer, body);
	}
	else if (header->GetMessageType() == IMessageHeader::TEMPLATE){
		int templateId = header->GetTemplateId();
		decode_body_succeed = MessageBodyUpdateCodec::Decode(buffer, body, context, templateId);
	}
	else{
		assert(0);
		decode_body_succeed = false;
	}

	if (!decode_body_succeed) {
		delete tmp_msg;
		return false;
	}

	message = tmp_msg;
	return true;
}

}
}