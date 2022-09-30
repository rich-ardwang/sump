#pragma once
#include "session.h"

namespace sfa {
namespace dds {

class IAMSession : public ISession {
public:
	virtual bool Init(void) override;
	virtual void SetAutoReconnect(bool is_auto, int try_times);
	virtual void Connect(const ClientInfo& client_info,
		const IEventListener* listener) override;
	virtual void Login(void) override;
	virtual void Logout(void) override;
	virtual void Uninit(void) override;
};

}
}
