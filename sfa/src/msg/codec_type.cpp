#include "codec_type.h"

namespace sfa {
namespace msg {

//停止位，0x80即1000 0000
const Byte STOP_BIT = (unsigned char)0x80;

//数据编码成功后，将最后一个字节的首位设置成1，即为停止位
ByteArray TypeCodec::Encode(const IFieldValue* value){
	ByteArray encoding = EncodeValue(value);
	if (!encoding.empty()) {
		encoding[encoding.size() - 1] |= STOP_BIT;
	}
	return encoding;
}

}
}
