#pragma once
#include <sfa/dds/dds_proxy.h>
#include "session.h"
#include "context.h"
#include "template.h"

namespace sfa {
namespace dds {

class DdsProxyImpl : public IDdsProxy {
public:
	DdsProxyImpl(ISession *session_dds, Context* context);
	virtual ~DdsProxyImpl();

	virtual uint32_t AsynRequest(const msg::IMessage* msg_req, const IMsgListener* listener) override;
	virtual bool Request(const msg::IMessage* msg_req, msg::IMessage*& msg_reply, int32_t timeout_seconds = 3) override;
	virtual bool Subscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) override;
	virtual void Unsubscribe(const char* ic, const std::vector<int> &filter_fields, const IMsgListener* listener) override;

private:
	class RequestManager* request_mgr_ = nullptr;
	Template* template_ = nullptr;
};

}
}