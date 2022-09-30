#include "dds_utils.h"
#include "lava_utils_api.h"
#include "sfa_utils/singleton.h"

namespace sfa {
namespace dds {

i_subscribe_manager *DdsUtils::get_subscribe_manager() {
	return sfa::utils::singleton_lazy<subscribe_manager, i_subscribe_manager>::instance();
}

}
}