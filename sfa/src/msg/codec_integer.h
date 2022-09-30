#pragma once
#include "codec_type.h"

namespace sfa {
namespace msg {

class IntegerCodec : public TypeCodec{
public:
	//�ж��޷�������Ҫռ�õ��ֽ���
	static int GetUnsignedIntegerSize(uint64_t value);

	//�ж��з�������Ҫռ�õ��ֽ�
	static int GetSignedIntegerSize(int64_t value);
};

}
}