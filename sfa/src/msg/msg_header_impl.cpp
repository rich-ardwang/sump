#include "msg_header_impl.h"
#include <algorithm>
#include <assert.h>

namespace sfa {
namespace msg {

MessageHeader::MessageType MessageHeader::GetMessageType(void) const {
	return msg_type_;
}

void MessageHeader::SetMessageType(MessageType msg_type) {
	msg_type_ = msg_type;
}

int MessageHeader::GetTemplateId(void) const {
	return template_id_;
}

void MessageHeader::SetTemplateId(int template_id) {
	template_id_ = template_id;
}

int MessageHeader::GetVersion(void) const {
	return version_;
}

void MessageHeader::SetVersion(int version) {
	version_ = version;
}

IFieldValue* MessageHeader::GetExtraField(int fid) const {
	return extra_fields_.Get(fid);
}

void MessageHeader::AddExtraField(int fid, const IFieldValue* value) {
	assert(value != nullptr);
	extra_fields_.Add(fid, value);
}

void MessageHeader::DelExtraField(int fid) {
	extra_fields_.Del(fid);
}

int MessageHeader::GetExtraFieldSize(void) const {
	return extra_fields_.Size();
}

std::vector<int> MessageHeader::GetAllExtraFieldId(void) const {
	std::vector<int> fids = extra_fields_.GetAllFieldId();
	//排序保证大小顺序，方便模板建立存在图
	std::sort(fids.begin(), fids.end());
	return fids;
}

IMessageHeader* MessageHeader::Clone(void) {
	return new MessageHeader(*this);
}

std::string MessageHeader::ToString(void) {
	std::string msg_type;
	switch (msg_type_) {
	case FULL:
		msg_type = "FULL";
		break;
	case INCREMENTAL:
		msg_type = "INCREMENTAL";
		break;
	case TEMPLATE:
		msg_type = "TEMPLATE";
		break;
	case UNKNOWN:
		msg_type = "UNKNOWN";
		break;
	default:
		msg_type = std::to_string(msg_type_);
		break;
	}

	std::string params;
	for (int fid : GetAllExtraFieldId()) {
		params += "fid:" + std::to_string(fid);
		params += ",value:" + GetExtraField(fid)->ToString() + " ";
	}

	return "[messageType=" + msg_type
		+ "\r\ntemplateId=" + std::to_string(template_id_)
		+ "\r\nversion=" + std::to_string(version_)
		+ "\r\nextraparam=" + params + "]";
}

}
}
