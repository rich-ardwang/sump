#pragma once
#include <sfa/msg/msg_def.h>

namespace sfa {
namespace msg {

const static char* VERSION = "0.1 Build(190902)";	//版本号

const static Byte MESSAGE_FLAG = 0x10;	//消息标示码

const static Byte MESSAGE_TYPE_INCREMENTAL = 0x01; //消息类型：自适应增量消息
const static Byte MESSAGE_TYPE_FULL = 0x02; //消息类型：自适应全量消息
const static Byte MESSAGE_TYPE_TEMPLATE = 0x03;	//消息类型：模板消息

const static Byte STOP_YES = 0x01; //停止位：停止
const static Byte STOP_NO = 0x00; //停止位：继续

const static Byte BYTE_YES = 0x01; //存在
const static Byte BYTE_NO = 0x00; //不存在

const static Byte FIELDTYPE_INT64 = 0x01; //Int64
const static Byte FIELDTYPE_UINT64 = 0x02; //UInt64
const static Byte FIELDTYPE_DECIMAL = 0x03; //十进制小数
const static Byte FIELDTYPE_ASCII = 0x04; //ASCII
const static Byte FIELDTYPE_UNICODE = 0x05; //Unicode
const static Byte FIELDTYPE_BYTE = 0x06; //Byte
const static Byte FIELDTYPE_DATE = 0x07; //日期
const static Byte FIELDTYPE_TIME = 0x08; //时间
const static Byte FIELDTYPE_ARRAY_BEGIN = 0x09; //数组开始
const static Byte FIELDTYPE_ARRAY_NEWROW = 0x0A; //数组增量开始
const static Byte FIELDTYPE_ARRAY_END = 0x0B; //数组结束
const static Byte FIELDTYPE_KVARRAY_BEGIN = 0x0C; //KV数组开始
const static Byte FIELDTYPE_KVARRAY_NEWROW = 0x0D; //KV数组增量开始
const static Byte FIELDTYPE_KVARRAY_END = 0x0E; //KV数组结束

const static Byte FIELDVALUE_RESERVED = 0x00; //保留
const static Byte FIELDVALUE_NULL = 0x01; //占位符(用于充斥模板)
const static Byte FIELDVALUE_NORMAL = 0x02; //常规
const static Byte FIELDVALUE_PAID = 0x03; //收费
const static Byte FIELDVALUE_CORRECT = 0x04; //纠错
const static Byte FIELDVALUE_WITHDRAW = 0x05; //撤回

}
}
