#include <stdint.h>
#include <array>
#include <string>
#include <iomanip>

class uuid
{
public:
	uuid();
	~uuid();
	static uuid generate_random();
	std::string to_string(const std::string& split = "-", bool up = true);
private:
	typedef uint8_t ValueType;
	static const size_t DATA_LENGTH = 16;
	friend bool operator==(uuid const& lhs, uuid const& rhs);
	friend bool operator<(uuid const& lhs, uuid const& rhs);

	std::array<ValueType, DATA_LENGTH> _data;
};