#pragma once
#include <string>
#include <sfa/msg/msg_def.h>

namespace sfa {
namespace msg {

//字段类型
enum class FieldType{
	INTEGER,		 //整型
	UNSIGNED_INTEGER,//无符号整型
	DECIMAL,		 //十进制
	ASCII,			 //Ascii
	UNIC0DE,		 //Unicode
	BYTE,			 //字节
	DATE,			 //日期
	TIME,			 //时间
	ARRAY,			 //数组
	KV_ARRAY		 //Key-value数组
};

//字段描述
enum class FieldDesc{
	NORMAL,			 //常规
	Null,			 //占位符(用于充斥模板)
	PAID,			 //收费
	CORRECT,		 //纠错
	WITHDRAW,		 //撤回
	RESERVED		 //保留
};

/**
 *
 * 基本字段接口
 * 各种类型字段实现此接口
 *
*/
class SFA_API IFieldValue{
public:
	virtual FieldType GetFieldType(void) const = 0;
	virtual FieldDesc GetFieldDesc(void) const = 0;

	virtual int64_t ToInt(void) = 0;
	virtual uint64_t ToUInt(void) = 0;
	virtual std::string ToString(void) = 0;
	virtual ByteArray ToBytes(void) = 0;

	virtual IFieldValue* Clone(void) = 0;
};

}
}
