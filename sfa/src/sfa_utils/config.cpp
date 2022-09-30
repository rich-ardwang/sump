#include "config.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include <filesystem>
#include <thread>
#include <chrono>
#include "sfa_log.h"
#include "sfa_utils.h"


namespace sfa {
namespace utils {

const unsigned CHECK_INTERVAL = 5; //every 5 sec to load the config

class Config::ConfigImpl {
public:
	ConfigImpl();
	bool init(const string &settingName);
	bool fail() { return !init_; }
	const string get(const string &key) const;
	const vector<string> values(const string &key);
	int count(const string &key);
	void run();

private:
	void clear();

private:
	bool		modified_;
	bool		init_;
	time_t		lastUpdateTime_;
	string		settingName_;
	map<string, vector<string> >	keyvalues_;
	static string					emptyString_; //for false return value
	static vector<string>			emptyArray_;
};

string Config::ConfigImpl::emptyString_;
vector<string> Config::ConfigImpl::emptyArray_;
std::mutex mutex_;

Config::ConfigImpl::ConfigImpl() 
	: init_(false)
	, lastUpdateTime_(0)
	, modified_(true)
{}

bool Config::ConfigImpl::init(const string &settingName) {
	settingName_ = settingName;
	time_t updateTime = 0;
	std::tr2::sys::path path(settingName_);
	try {
		updateTime = std::tr2::sys::last_write_time(path);
		conf_autolock write_guard(mutex_);
		if (lastUpdateTime_ == updateTime) {
			modified_ = false;
			return true;
		}
		else {
			lastUpdateTime_ = updateTime;
			modified_ = true;
		}
	}
	catch (std::tr2::sys::filesystem_error e) {
		sfa_log_error("sfa.config", "Config::ConfigImpl::init: %s", e.what());
	}

	std::ifstream in(settingName_.c_str(), std::ios::in);
	if (in.fail()) {
		sfa_log_error("sfa.config", "Config::ConfigImpl::init: %s open fail", settingName_.c_str());
		init_ = false;
		return false;
	}

	clear();
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::string::size_type pos = line.find("=");
		if (pos == std::string::npos) {
			continue;
		}
		std::string title = line.substr(0, pos);
		std::string content = line.substr(pos + 1);
		sfa::utils::CUtils::trim(title);
		sfa::utils::CUtils::trim(content);
		keyvalues_[title].push_back(content);
	}

	init_ = true;
	sfa_log_info("sfa.config", "Config::ConfigImpl::init:  %s updated", settingName_.c_str());
	return true;
}

void Config::ConfigImpl::run() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(CHECK_INTERVAL));
		init(settingName_);
	}
}

void Config::ConfigImpl::clear() {
	conf_autolock write_guard(mutex_);
	keyvalues_.clear();
}

const string Config::ConfigImpl::get(const string &key) const {
	conf_autolock write_guard(mutex_);
	map<string, vector<string> >::const_iterator it = keyvalues_.find(key);
	if (it == keyvalues_.end()) {
		return emptyString_;
	}
	else {
		return it->second[0];
	}
}

const vector<string> Config::ConfigImpl::values(const string &key) {
	conf_autolock write_guard(mutex_);
	if (keyvalues_.count(key)) {
		return keyvalues_[key];
	}
	return emptyArray_;
}

int Config::ConfigImpl::count(const string &key) {
	conf_autolock write_guard(mutex_);
	if (0 == keyvalues_.count(key)) {
		return 0;
	}
	return keyvalues_[key].size();
}

///////////////////////////////////////////////////////////////////////////////
// class Config implement
// 定时加载配置文件线程
///////////////////////////////////////////////////////////////////////////////
Config::Config() : configs_(new ConfigImpl()) {}

Config::~Config() {
	delete configs_;
}

Config& Config::instance() {
	static Config configs;
	return configs;
}

bool Config::init(const string &settingName) {
	bool ret = configs_->init(settingName);
	if (ret) {
		static std::thread threadLoadConfig = 
			std::thread(std::bind(&Config::ConfigImpl::run, configs_));
	}
	return ret;
}

bool Config::fail() const {
	return configs_->fail();
}

const string Config::value(const string &key) const {
	return configs_->get(key);
}

const vector<string> Config::values(const string &key) const {
	return configs_->values(key);
}

int Config::count(const string &key) const {
	return configs_->count(key);
}

}
}
