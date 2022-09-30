#include "codec_ascii_string.h"
#include <sfa/msg/value_ascii.h>
#include <assert.h>

namespace sfa {
namespace msg {

ByteArray AsciiStringCodec::EncodeValue(const IFieldValue* value){
	assert(value != nullptr);
	std::string str = const_cast<IFieldValue*>(value)->ToString();	
	ByteArray btBuffer;
	for (size_t i=0; i<str.size(); i++){
		btBuffer.push_back(str[i]);
	}
	return btBuffer;
}

IFieldValue* AsciiStringCodec::Decode(ByteArray& buffer){
	assert(!buffer.empty());
	if (buffer.empty()) {
		return new AsciiValue("");
	}

	int byt = 0;
	ByteArray btBuffer;

	do{
		 byt = buffer[0];
		 buffer.erase(buffer.begin());
		 btBuffer.push_back(byt);
	} while ((byt&0x80)==0 && !buffer.empty());

	//复原最后一个字节为真实数据
	btBuffer[btBuffer.size() - 1] &= 0x7f;

	if (btBuffer[0] == 0){
		if (!AsciiStringCodec::IsEmpty(btBuffer)){//字符串超长
			return nullptr;
		}
		return new AsciiValue("");
	}

	std::string str;
	for (size_t i=0; i<btBuffer.size(); i++){
		str += btBuffer[i];
	}
	return new AsciiValue(std::string(str));
}

bool AsciiStringCodec::IsEmpty(const ByteArray& bytes){
	for (size_t i=0; i<bytes.size(); i++){
		if ((bytes[i]&0x7f) != 0){
			return false;
		}
	}
	return true;
}

}
}