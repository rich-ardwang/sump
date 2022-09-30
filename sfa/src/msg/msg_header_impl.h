#pragma once
#include <map>
#include <sfa/msg/msg_header.h>
#include "fields.h"

namespace sfa {
namespace msg {

/**
*
* 消息头，由下列字段组成:
*	消息标识码（必须）1字节，固定为0x10
*
*	状态字段（必须）  1字节
*		停止位        [0]	1位
*		消息类型      [1-2] 2位，模板消息0x03、全量消息0x02、增量消息0x01
*		存在模板编号  [3]   1位，0不存在、1存在
*		存在模板版本号[4]   1位，0不存在、1存在
*		存在附加参数  [5]   1位，0不存在、1存在
*
*	模板编号（可选）  n字节，采用停止位整型编码
*	模板版本号（可选）n字节，采用停止位整型编码
*	附加参数（可选）  n字节，同消息体字段编码
*
*/
class MessageHeader : public IMessageHeader {
public:
	MessageType GetMessageType(void) const override;
	void SetMessageType(MessageType msg_type) override;

	int GetTemplateId(void) const override;
	void SetTemplateId(int template_id) override;

	int GetVersion(void) const override;
	void SetVersion(int version) override;

	IFieldValue* GetExtraField(int fid) const override;
	void AddExtraField(int fid, const IFieldValue* value) override;
	void DelExtraField(int fid) override;

	std::vector<int> GetAllExtraFieldId(void) const override;
	int GetExtraFieldSize(void) const override;

	std::string ToString(void) override;
	IMessageHeader* Clone(void) override;

private:
	MessageType msg_type_ = UNKNOWN; //消息类型:全量、增量、模板
	int template_id_ = 0; //模板编号
	int version_ = 0; //模板版本号
	Fields extra_fields_; //附加参数
};

}
}