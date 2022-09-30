#pragma once

namespace sfa {
namespace msg {
namespace utils {

template<typename T>
void DeletePtr(T** ptr) {
	if (ptr != nullptr) {
		if (*ptr != nullptr) {
			delete* ptr;
			*ptr = nullptr;
		}
		ptr = nullptr;
	}
}

}
}
}