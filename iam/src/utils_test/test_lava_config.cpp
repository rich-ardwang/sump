#include <thread>
#include <chrono>
#include <string>
#include <assert.h>
#include "lava_utils_api.h"

#define test_lava_config_moduler	"lava.test_lava_config"
extern lava_utils_api lava::utils::i_lava_config *get_lava_config();
extern lava_utils_api void release_lava_config();


void test_lava_config() {
	/* config init */
	get_lava_config()->init();
	log_info(test_lava_config_moduler, "initial config complete.");

	/* wait until loading config complete */
	while (get_lava_config()->fail())
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	log_info(test_lava_config_moduler, "loading config complete.");

	/* get msgbus config info */
	char *ret_buf = nullptr;
	int ret_len = 0;
	get_lava_config()->value("msgbus.direct.ip", ret_buf, ret_len);
	std::string drt_ip(ret_buf);
	get_lava_config()->value("msgbus.direct.listen_queue", ret_buf, ret_len);
	std::string drt_lsn_q(ret_buf);
	get_lava_config()->value("msgbus.fanout.ip", ret_buf, ret_len);
	std::string fanout_ip(ret_buf);
	get_lava_config()->value("msgbus.fanout.listen_queue", ret_buf, ret_len);
	std::string fanout_lsn_q(ret_buf);
	if (ret_buf)
	{
		delete[] ret_buf;
		ret_buf = nullptr;
	}
	assert(!drt_ip.empty());
	assert(!drt_lsn_q.empty());
	assert(!fanout_ip.empty());
	assert(!fanout_lsn_q.empty());

	/* output conf data */
	log_info(test_lava_config_moduler, "ip=%s, lsn_q=%s, f_ip=%s, f_lsn_q=%s", 
		drt_ip.c_str(), drt_lsn_q.c_str(), 
		fanout_ip.c_str(), fanout_lsn_q.c_str());

	/* release lava_config module */
	/* it will wait the conf_impl thread stopped and then release the object. */
	release_lava_config();
}
