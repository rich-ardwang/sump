#pragma once
#include <sfa/dds/dds_def.h>
#include "client_info.h"

namespace sfa {
namespace dds {

class Context {
public:
	Context();
	virtual ~Context();

	inline msg::IMessageContext* GetMsgContext(void) const {
		return msg_context_;
	}

	inline ClientInfo* GetClientInfo(void) {
		return &client_info_;
	}

private:
	Context& operator=(const Context&) = delete;
	Context(const Context&) = delete;

private:
	ClientInfo client_info_; //账号信息
	msg::IMessageContext* msg_context_ = nullptr; //sdn messag上下文
};

}
}