#include "codec_signed_integer.h"
#include <assert.h>
#include <sfa/msg/value_integer.h>

namespace sfa {
namespace msg {

ByteArray SignedIntegerCodec::EncodeValue(const IFieldValue* value){
	assert(value != nullptr);
	int64_t longValue = const_cast<IFieldValue*>(value)->ToInt();
	return EncodeValue(longValue);
}

ByteArray SignedIntegerCodec::EncodeValue(int64_t longValue){
	int size = GetSignedIntegerSize(longValue);
	ByteArray encoding;
	encoding.resize(size);

	// 组装数据，即每个字节第一位不表示数据；组装完成后仍然是大端序列(低字节位为值得高有效位)
	for (int factor = 0; factor < size; factor++) {
		// 0x3f = 0011 1111
		// 0x7f = 0111 1111
		int bitMask = (factor == (size - 1)) ? 0x3f : 0x7f;
		encoding[size - factor - 1] = (Byte)((longValue >> (factor * 7)) & bitMask);
	}
	// Get the sign bit from the long value and set it on the first byte
	// 01000000 00000000 ... 00000000
	// ^----SIGN BIT
	// 将第0个字节的第二位设置为符号位， 0表示正数 1表示负数
	encoding[0] |= (0x40 & (longValue >> 57));// 0x40 = 0100 0000
	return encoding;
}

IFieldValue* SignedIntegerCodec::Decode(ByteArray& buffer){
	assert(!buffer.empty());

	int64_t value = 0;

	//通过首字节的第二位与运算，确认该数据的符号
	int byt = buffer[0];
	buffer.erase(buffer.begin());
	if ((byt & 0x40) > 0){
		value = -1;
	}
	// 到此，value的符号已经确定，
	// value=0 则该数为负数 value= -1该数为正数
	// int value = -1 16进制 0xFF FF FF FF
	// int value = 0 16进制 0x00 00 00 00
	// 下面的只是通过位操作来复原真实的数据
	value = (value << 7) | (byt & 0x7f);

	while ((byt&0x80)==0 && !buffer.empty()){ //根据第一位来判断当前byte是否属于这个字段
		byt = buffer[0];
		buffer.erase(buffer.begin());
		value = (value<<7) | (byt&0x7f); //先把有效位往左移7位，然后再处理当前的七位
	}

	return new IntegerValue(value);
}

}
}