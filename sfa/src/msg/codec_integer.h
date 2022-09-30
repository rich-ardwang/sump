#pragma once
#include "codec_type.h"

namespace sfa {
namespace msg {

class IntegerCodec : public TypeCodec{
public:
	//判断无符号数所要占用的字节数
	static int GetUnsignedIntegerSize(uint64_t value);

	//判断有符号数需要占用的字节
	static int GetSignedIntegerSize(int64_t value);
};

}
}