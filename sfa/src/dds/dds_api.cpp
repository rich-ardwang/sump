#pragma once
#include <sfa/dds/dds_api.h>
#include "dds_api_impl.h"

namespace sfa {
namespace dds {

SFA_API bool Init(_sfa_in_ const char* config_file) {
	return DdsApiImpl::GetInstance().Init(config_file);
}

SFA_API void SetAutoReconnect(_sfa_in_ bool is_auto, _sfa_in_ int try_times) {
	DdsApiImpl::GetInstance().SetAutoReconnect(is_auto, try_times);
}

SFA_API void Login(_sfa_in_ const ClientLoginInfo* login_info, _sfa_in_ const IEventListener* listener) {
	DdsApiImpl::GetInstance().Login(login_info, listener);
}

SFA_API void Logout(void) {
	DdsApiImpl::GetInstance().Logout();
}

SFA_API IDdsProxy* CreateProxy(void) {
	return DdsApiImpl::GetInstance().CreateProxy();
}

SFA_API void ReleaseProxy(_sfa_inout_ IDdsProxy** proxy) {
	DdsApiImpl::GetInstance().ReleaseProxy(proxy);
}

SFA_API void Uninit(void) {
	DdsApiImpl::GetInstance().Uninit();
}

}
}