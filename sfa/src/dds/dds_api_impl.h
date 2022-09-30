#pragma once
#include <mutex>
#include <sfa/dds/dds_proxy.h>
#include <sfa/dds/dds_listener.h>
#include "session.h"
#include "context.h"

namespace sfa {
namespace dds {

class DdsApiImpl : public IEventListener {
public:
	static DdsApiImpl& GetInstance(void);

	bool Init(const char* config_file);
	void SetAutoReconnect(bool is_auto, int try_times);
	void Login(const ClientLoginInfo* login_info,
		const IEventListener* listener);
	IDdsProxy* CreateProxy(void);
	void ReleaseProxy(IDdsProxy** proxy);
	void Logout(void);
	void Uninit(void);

public:
	virtual void OnEvent(uint16_t event) override;

private:
	DdsApiImpl();
	virtual ~DdsApiImpl();
	DdsApiImpl& operator=(const DdsApiImpl&) = delete;
	DdsApiImpl(const DdsApiImpl&) = delete;

	void ConnectIAM(void);
	void LoginIAM(void);
	void LogoutIAM(void);
	void ConnectDDS(void);
	void LoginDDS(void);
	void NotifyListener(uint16_t event);

private:
	bool inited_ = false;
	bool logined_ = false;
	bool is_auto_reconnect_ = true;
	int try_times_ = 3;
	std::mutex mutex_tmpl;

	ISession* session_iam_ = nullptr;
	ISession* session_dds_ = nullptr;
	IEventListener* event_listener_ = nullptr;
	Context* context_ = nullptr;
};

}
}