/***************************************************************************************************
*Copyright(C)	: 2019, Sumscope
*FileName		: Config.h
*Author			: lei.wang@sumscope.com
*Version		: 1.0
*Date			: 2019-08-19
*Desc			: This class completes config file loading and provides an interface
*				: for users to query.
*History		: Add by Lei.Wang 2019-08-19
***************************************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "sfa_base.h"

using std::string;
using std::vector;
using std::map;
typedef sfa::base::scopelocker<std::mutex>	conf_autolock;


namespace sfa {
namespace utils {

class Config : sfa::base::noncopyable {
public:
	static Config& instance();
	~Config();

	/*
	 * init
	 * @param   settingName : config file name
	 * @return  init result : true->success | false-failed
	 */
	bool init(const string &settingName = "setting.ini");

	/*
	 * Indicates whether the init was successful
	 * @return  true->success | false-failed
	 */
	bool fail() const;

	/**
	 * get value by the key
	 * @param   key   [in] key in config file
	 * @return  value, if not find, return empty string, if multi value, return the first string
	 */
	const string value(const string &key) const;

	/*
	 * get all values by the key
	 * @param   key   [in] key in config file
	 * @return  all values, if the value is empty, return empty vector
	 */
	const vector<string> values(const string &key) const;

	/**
	 * get the count of the values which are pointed by the key
	 * @param   key   [in] key in config file
	 * @return  the count of the values
	 */
	int count(const string &key) const;

private:
	Config();

private:
	class ConfigImpl;
	ConfigImpl* configs_;
};

}
}
