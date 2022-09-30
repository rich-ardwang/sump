#pragma once
#include <vector>
#include <map>
#include <sfa/msg/msg_body.h>
#include <sfa/msg/value_array.h>
#include <sfa/msg/value_kv_array.h>
#include "fields.h"

namespace sfa {
namespace msg {

/**
*
* 消息体，消息体分：全量体、增量消息体。
* 其中全量消息体由n个数据段组成、增量消息由1个存在图(PMAP，n个字节)和n个数据段组成。
* 每个数据段由下列字段组成：
*	字段描述 1字节
*		字段类型[0-4] 5位:
*			[0x01]Int64
*			[0x02]UInt64
*			[0x03]十进制小数
*			[0x04]ASCII
*			[0x05]Unicode
*			[0x06]Byte
*			[0x07]日期
*			[0x08]时间
*			[0x09]数组开始
*			[0x0A]数组增量开始
*			[0x0B]数组结束
*			[0x0C]KV数组开始
*			[0x0D]KV数组增量开始
*			[0x0E]KV数组结束
*		字段描述[5-7] 3位:
*			[0x0]保留
*			[0x1]占位符(用于充斥模板)
*			[0x2]常规
*			[0x3]收费
*			[0x4]纠错
*			[0x5]撤回
*
*	字段FID  n字节，采用停止位整型编码
*	字段内容 n字节
*
*/
class MessageBody : public IMessageBody {
public:
	void AddField(int fid, const IFieldValue* value) override;
	IFieldValue* GetField(int fid) const override;
	void DelField(int fid) override;

	std::vector<int> GetAllFieldId(void) const override;
	int GetFieldSize(void) const override;

	std::string ToString(void) override;
	IMessageBody* Clone(void) override;

private:
	std::string ShowArray(ArrayValue* value);
	std::string ShowKVArray(KVArrayValue* value);
	std::string ShowField(IFieldValue* value);
	std::string GetFieldTypeStr(FieldType type);

private:
	Fields fields_; //字段
};

}
}