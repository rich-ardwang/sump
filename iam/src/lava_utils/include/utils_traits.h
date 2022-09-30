#pragma once
#include <stdint.h>

namespace lava
{
	namespace utils
	{
		struct default_traits
		{
			typedef uint32_t count_type;
			static const count_type ITEM_COUNT = 64;
		};
	}
}