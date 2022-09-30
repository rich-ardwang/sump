#include "codec_type.h"

namespace sfa {
namespace msg {

//ֹͣλ��0x80��1000 0000
const Byte STOP_BIT = (unsigned char)0x80;

//���ݱ���ɹ��󣬽����һ���ֽڵ���λ���ó�1����Ϊֹͣλ
ByteArray TypeCodec::Encode(const IFieldValue* value){
	ByteArray encoding = EncodeValue(value);
	if (!encoding.empty()) {
		encoding[encoding.size() - 1] |= STOP_BIT;
	}
	return encoding;
}

}
}
