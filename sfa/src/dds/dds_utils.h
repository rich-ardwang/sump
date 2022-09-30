#pragma once 
#include <lava/lava_comm/lava_comm_api.h>
#include <sfa/dds/dds_def.h>
#include "subscribe_manager.h"

namespace sfa {
namespace dds {

class DdsUtils{
public:
	template<typename T>
	static void DeletePtr(T** ptr) {
		if (ptr != nullptr) {
			if (*ptr != nullptr) {
				delete* ptr;
				*ptr = nullptr;
			}
			ptr = nullptr;
		}
	}

	static i_subscribe_manager *get_subscribe_manager();
};

}
}
