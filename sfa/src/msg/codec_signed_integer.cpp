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

	// ��װ���ݣ���ÿ���ֽڵ�һλ����ʾ���ݣ���װ��ɺ���Ȼ�Ǵ������(���ֽ�λΪֵ�ø���Чλ)
	for (int factor = 0; factor < size; factor++) {
		// 0x3f = 0011 1111
		// 0x7f = 0111 1111
		int bitMask = (factor == (size - 1)) ? 0x3f : 0x7f;
		encoding[size - factor - 1] = (Byte)((longValue >> (factor * 7)) & bitMask);
	}
	// Get the sign bit from the long value and set it on the first byte
	// 01000000 00000000 ... 00000000
	// ^----SIGN BIT
	// ����0���ֽڵĵڶ�λ����Ϊ����λ�� 0��ʾ���� 1��ʾ����
	encoding[0] |= (0x40 & (longValue >> 57));// 0x40 = 0100 0000
	return encoding;
}

IFieldValue* SignedIntegerCodec::Decode(ByteArray& buffer){
	assert(!buffer.empty());

	int64_t value = 0;

	//ͨ�����ֽڵĵڶ�λ�����㣬ȷ�ϸ����ݵķ���
	int byt = buffer[0];
	buffer.erase(buffer.begin());
	if ((byt & 0x40) > 0){
		value = -1;
	}
	// ���ˣ�value�ķ����Ѿ�ȷ����
	// value=0 �����Ϊ���� value= -1����Ϊ����
	// int value = -1 16���� 0xFF FF FF FF
	// int value = 0 16���� 0x00 00 00 00
	// �����ֻ��ͨ��λ��������ԭ��ʵ������
	value = (value << 7) | (byt & 0x7f);

	while ((byt&0x80)==0 && !buffer.empty()){ //���ݵ�һλ���жϵ�ǰbyte�Ƿ���������ֶ�
		byt = buffer[0];
		buffer.erase(buffer.begin());
		value = (value<<7) | (byt&0x7f); //�Ȱ���Чλ������7λ��Ȼ���ٴ���ǰ����λ
	}

	return new IntegerValue(value);
}

}
}