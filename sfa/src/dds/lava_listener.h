#pragma once
#include <atomic>
#include <lava/lava_comm/i_message_listener.h>

namespace sfa {
namespace dds {

template<typename T>
class LavaListener : public lava::comm::i_message_listener {
public:
	explicit LavaListener(T* proxy) : proxy_(proxy) {}
	virtual ~LavaListener() {}

	virtual void on_reply(__lv_in lava::comm::i_message* msg, __lv_in const char* topic) override {
		if (proxy_ != nullptr) {
			proxy_->OnReply(msg, topic);
		}
	}

	virtual void on_message(__lv_in lava::comm::i_message* msg) override {
		if (proxy_ != nullptr) {
			proxy_->OnMessage(msg);
		}
	}

private:
	T* proxy_ = nullptr;
	i_object_impl(std::atomic<int32_t>)
};

}
}