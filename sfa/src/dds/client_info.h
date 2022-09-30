#pragma once 
#include <string>
#include <sfa/dds/dds_def.h>

namespace sfa{
namespace dds{

typedef struct {
	ProxyType type;
	std::string host;
	std::string port;
	std::string username;
	std::string password;
}ProxyInfo;

typedef struct {
	bool use_proxy;
	ProxyInfo* proxy_info;
	std::string host;
	std::string port;
	std::string username;
	std::string password;
}LoginInfo;

class ClientInfo {
public:
	ClientInfo();
	~ClientInfo();

	inline LoginInfo* GetLoginInfo(void) {
		return &login_info_;
	}

	void SetUserInfo(const char* username, const char* password);
	void GetUserInfo(std::string& username, std::string& password);
	void SetServerInfo(const char* host, const char* port);
	void GetServerInfo(std::string& host, std::string& port);
	void SetProxyInfo(ClientProxyInfo* proxy_info);
	void GetConnectInfo(std::string& host, std::string& port);

	inline const char* GetUserName(void) const {
		return login_info_.username.c_str();
	}
	
	inline const char* GetPassword(void) const {
		return login_info_.password.c_str();
	}
	
	inline const char* GetHost(void) const {
		return login_info_.host.c_str();
	}

	inline const char* GetPort(void) const {
		return login_info_.port.c_str();
	}
		
private:
	LoginInfo login_info_;
};

}
}


