#include "codec_unicode_string.h"
#include <string>
#include <assert.h>
#include <sfa/msg/value_unicode.h>
#include <sfa/msg/value_byte.h>
#include "utils_ansi_unicode_utf8.h"

namespace sfa {
namespace msg {

ByteArray UnicodeStringCodec::Encode(const IFieldValue* value){
	assert(value != nullptr);
	std::string str = const_cast<IFieldValue*>(value)->ToString();
	std::string strUtf8 = utils::AnsiToUtf8(str);

	ByteArray utf8encoding;
	for (size_t i = 0; i<strUtf8.size(); i++){
		utf8encoding.push_back(strUtf8.at(i));
	}

	ByteValue tmpVal(utf8encoding);
	return ByteCodec::Encode(&tmpVal);
}

IFieldValue* UnicodeStringCodec::Decode(ByteArray& buffer){
	assert(!buffer.empty());

	ByteCodec codec;
	ByteValue* value = (ByteValue*)codec.Decode(buffer);
	if (value != nullptr){
		std::string str;
		ByteArray bytValue = value->ToBytes();
		assert(!bytValue.empty());
		for (size_t i=0; i<bytValue.size(); i++){
			str += bytValue[i];
		}
		delete value;
		std::string strAnsi = utils::Utf8ToAnsi(str);
		return new UnicodeValue(strAnsi);
	}
	return nullptr;
}

}
}