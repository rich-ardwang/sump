#include "client_info.h"

namespace sfa {
namespace dds {

const int MAX_USERNAME_LEN = 64;
const int MAX_PASSWORD_LEN = 64;

ClientInfo::ClientInfo() {
	login_info_.use_proxy = false;
	login_info_.proxy_info = nullptr;
}

ClientInfo::~ClientInfo() {
	if (login_info_.proxy_info != nullptr) {
		delete login_info_.proxy_info;
		login_info_.proxy_info = nullptr;
	}
}

void ClientInfo::SetUserInfo(const char* username, const char* password) {
	if (username== nullptr || password==nullptr) {
		return;
	}

	if (strlen(username) >= MAX_USERNAME_LEN) {
		return;
	}

	if (strlen(password) >= MAX_PASSWORD_LEN) {
		return;
	}

	login_info_.username = username;
	login_info_.password = password;
}

void ClientInfo::GetUserInfo(std::string& user_name, std::string& password) {
	user_name = login_info_.username;
	password = login_info_.password;
}

void ClientInfo::SetServerInfo(const char* host, const char* port) {
	if (host==nullptr || port==nullptr) {
		return;
	}

	login_info_.host = host;
	login_info_.port = port;
}

void ClientInfo::GetServerInfo(std::string& host, std::string& port) {
	host = login_info_.host;
	port = login_info_.port;
}

void ClientInfo::SetProxyInfo(ClientProxyInfo* proxy_info) {
	if (proxy_info == nullptr) {
		return;
	}

	if (login_info_.proxy_info != nullptr) {
		delete login_info_.proxy_info;
		login_info_.proxy_info = nullptr;
	}

	login_info_.use_proxy = (proxy_info->type != ProxyType::proxy_none);
	login_info_.proxy_info = new(std::nothrow) ProxyInfo();
	if (login_info_.proxy_info == nullptr) {
		return;
	}

	login_info_.proxy_info->type = proxy_info->type;
	login_info_.proxy_info->host = proxy_info->host;
	login_info_.proxy_info->port = proxy_info->port;
	login_info_.proxy_info->username = proxy_info->username;
	login_info_.proxy_info->password = proxy_info->password;
}

void ClientInfo::GetConnectInfo(std::string& host_, std::string& port_) {
	if (login_info_.use_proxy) {
		host_ = login_info_.proxy_info->host;
		port_ = login_info_.proxy_info->port;
	}
	else{
		host_ = login_info_.host;
		port_ = login_info_.port;
	}
}

}
}


