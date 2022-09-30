#pragma once
#include <lava/lava_base/lava_base.h>
#include <lava/lava_box/lava_box_api.h>
#include "session.h"

namespace sfa {
namespace dds {

using namespace lava::comm;
using namespace lava::base;
using namespace lava::box;

class DdsSession : public ISession {
public:	
	virtual bool Init(void) override;
	virtual void SetAutoReconnect(bool is_auto, int try_times);
	virtual void Connect(const ClientInfo& client_info, 
		const IEventListener* listener) override;
	virtual void Login(void) override;
	virtual void Logout(void) override;
	virtual void Uninit(void) override;

	i_auto_ptr<i_proxy> CreateProxy(void);
 	void OnEvent(int32_t event);

private:
	client_login_info* GetLoginInfo(const ClientInfo& client_info);

private:
	bool is_auto_reconnect_ = true;
	int try_times_ = 3;
	i_auto_ptr<i_proxy> lava_proxy_ = nullptr;
	i_auto_ptr<i_event_listener> lava_listener_ = nullptr;
};

}
}
