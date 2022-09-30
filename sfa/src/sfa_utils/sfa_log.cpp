//
//  sfa_log.cpp
//
//  Re-Edited by lei.wang on 2019-08-09.
//  Copyright (c) 2019 Sumscope. All rights reserved.
//
#include "sfa_log.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/rollingfileappender.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/net/syslogappender.h>
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/helpers/exception.h>

#ifdef SFA_WIN
#include <Windows.h>
#include <io.h>
#include <tchar.h>
//#include <vld.h>
#include "sfa_base.h"
#define snprintf  sprintf_s
#else
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#endif

#pragma comment(lib, "log4cxx.lib")
using namespace log4cxx;


namespace sfa {
namespace utils {

class sfa_log_impl {
public:
	sfa_log_impl();
	~sfa_log_impl() {}

private:
	static void _initlog();
};

static const char default_logger_module[] = "sumscope.sfa";
static sfa_log_impl _log;
static bool inited = false;

static void initDefaultLog() {
	LayoutPtr layout(new PatternLayout(LOG4CXX_STR("%d -%5p [%t] [%c] %m%n")));
	//PatternLayout(const LogString& pattern);
	/*   设置控制台输出定义   */
	ConsoleAppenderPtr appender(new ConsoleAppender(layout));
	appender->setEncoding(LOG4CXX_STR("utf-8"));
	appender->setOption(LOG4CXX_STR("Encoding"), LOG4CXX_STR("utf-8"));
	log4cxx::helpers::Pool p;
	appender->activateOptions(p);
	LoggerPtr logger = Logger::getLogger(default_logger_module);
	logger->addAppender(appender);
	logger->setLevel(Level::getError());
	LogManager::getLoggerRepository()->setConfigured(true);
}

#ifdef SFA_WIN
static std::wstring getFormatedMsg(const wchar_t* msgfmt, va_list ap) {
	const int maxLen = 1024;
	wchar_t tmpInfo[maxLen];
	memset(tmpInfo, 0, maxLen * sizeof(wchar_t));
	int ret = _vsnwprintf_s(tmpInfo, maxLen, msgfmt, ap);
	return tmpInfo;
}
#endif
static std::string getFormatedMsg(const char* msgfmt, va_list ap) {
	const int maxLen = 2048;
	char tmpInfo[maxLen];
	memset(tmpInfo, 0, maxLen);

#ifdef SFA_WIN
	int ret = vsnprintf_s(tmpInfo, maxLen, msgfmt, ap);
#else
	int ret = vsnprintf(tmpInfo, maxLen, msgfmt, ap);
#endif
	return tmpInfo;
}

static void log(LoggerPtr logger, LogLevel level, const std::string &loginfo) {
	try {
		switch (level) {
		case Trace:
			LOG4CXX_TRACE(logger, loginfo);
			break;
		case Debug:
			LOG4CXX_DEBUG(logger, loginfo);
			break;
		case Info:
			LOG4CXX_INFO(logger, loginfo);
			break;
		case Warning:
			LOG4CXX_WARN(logger, loginfo);
			break;
		default:
			LOG4CXX_ERROR(logger, loginfo);
			break;
		}
	}
	catch (log4cxx::helpers::IOException&) {
	}
}

static void log(LoggerPtr logger, LogLevel level, const char* msgfmt, va_list ap) {
	std::string loginfo = getFormatedMsg(msgfmt, ap);
	try {
		switch (level) {
		case Trace:
			LOG4CXX_TRACE(logger, loginfo);
			break;
		case Debug:
			LOG4CXX_DEBUG(logger, loginfo);
			break;
		case Info:
			LOG4CXX_INFO(logger, loginfo);
			break;
		case Warning:
			LOG4CXX_WARN(logger, loginfo);
			break;
		default:
			LOG4CXX_ERROR(logger, loginfo);
			break;
		}
	}
	catch (log4cxx::helpers::IOException&) {
	}
}

sfa_log_impl::sfa_log_impl() {
	_initlog();
}

void sfa_log_impl::_initlog() {
	if (!inited) {
		std::string logConfigName;
#ifdef SFA_WIN
		std::string moduleFilePath = sfa::base::getModulePath();
		if (!moduleFilePath.empty()) {
			logConfigName = moduleFilePath + "\\log4cxx.properties";
		}
		else {
			logConfigName = ".\\log4cxx.properties";
		}
#else
		logConfigName = "log4cxx.properties";
#endif

#ifdef SFA_WIN
		int f = _access(logConfigName.c_str(), 0);
#else
		int f = access(logConfigName.c_str(), R_OK);
#endif
		if (f != -1) {
			PropertyConfigurator::configureAndWatch(logConfigName.c_str(), 5000);
			sfa_log4(default_logger_module, Info, "use log config file:%s", logConfigName.c_str());
			inited = true;
		}
		else {
			initDefaultLog();
			inited = true;
		}
	}
}

void sfa_log4(_sfa_in_ const char* module, _sfa_in_ LogLevel level, _sfa_in_ const char* msgfmt, ...) {
	bool needLog = false;
	LoggerPtr userlogger(Logger::getLogger(module));
	switch (level) {
	case Trace:
		needLog = userlogger->isTraceEnabled();
		break;
	case Debug:
		needLog = userlogger->isDebugEnabled();
		break;
	case Info:
		needLog = userlogger->isInfoEnabled();
		break;
	case Warning:
		needLog = userlogger->isWarnEnabled();
		break;
	default:
		needLog = true;
		break;
	}

	if (needLog) {
		va_list ap;
		va_start(ap, msgfmt);
		log(userlogger, level, msgfmt, ap);
		va_end(ap);
	}
}

void sfa_log4(_sfa_in_ const char* module, _sfa_in_ LogLevel level, _sfa_in_ const char* file, _sfa_in_ const char* function,
		_sfa_in_ const int line, _sfa_in_ const char* msgfmt, ...) {
	if (NULL == file || NULL == function || NULL == msgfmt) {
		return;
	}

	bool needLog = false;
	LoggerPtr userlogger(Logger::getLogger(module));
	if (!userlogger) {
		userlogger = Logger::getRootLogger();
	}
	if (!userlogger) {
		return;
	}

	switch (level) {
	case Trace:
		needLog = userlogger->isTraceEnabled();
		break;
	case Debug:
		needLog = userlogger->isDebugEnabled();
		break;
	case Info:
		needLog = userlogger->isInfoEnabled();
		break;
	case Warning:
		needLog = userlogger->isWarnEnabled();
		break;
	default:
		needLog = true;
		break;
	}

	if (needLog) {
		va_list ap;
		va_start(ap, msgfmt);
		char acTmp[30] = { 0 };
#ifdef SFA_WIN
		sprintf_s(acTmp, "%d", line);
#else
		sprintf_s(acTmp, sizeof(acTmp)-1, "%d", line);
#endif
		std::string strLog;
		strLog = "[" + std::string(file) + ":" + std::string(function) + "(" + std::string(acTmp) + ")] ";
		strLog += getFormatedMsg(msgfmt, ap);
		va_end(ap);
		log(userlogger, level, strLog);
	}
}

#ifdef SFA_WIN
static void log(LoggerPtr logger, LogLevel level, const std::wstring& loginfo) {
	try {
		switch (level) {
		case Trace:
			LOG4CXX_TRACE(logger, loginfo);
			break;
		case Debug:
			LOG4CXX_DEBUG(logger, loginfo);
			break;
		case Info:
			LOG4CXX_INFO(logger, loginfo);
			break;
		case Warning:
			LOG4CXX_WARN(logger, loginfo);
			break;
		default:
			LOG4CXX_ERROR(logger, loginfo);
			break;
		}
	}
	catch (log4cxx::helpers::IOException&) {
	}
}

static void log(LoggerPtr logger, LogLevel level, const wchar_t* msgfmt, va_list ap) {
	std::wstring loginfo = getFormatedMsg(msgfmt, ap);
	try {
		switch (level) {
		case Trace:
			LOG4CXX_TRACE(logger, loginfo);
			break;
		case Debug:
			LOG4CXX_DEBUG(logger, loginfo);
			break;
		case Info:
			LOG4CXX_INFO(logger, loginfo);
			break;
		case Warning:
			LOG4CXX_WARN(logger, loginfo);
			break;
		default:
			LOG4CXX_ERROR(logger, loginfo);
			break;
		}
	}
	catch (log4cxx::helpers::IOException&) {
	}
}

void sfa_log4(_sfa_in_ const wchar_t* module, _sfa_in_ LogLevel level, _sfa_in_ const wchar_t* msgfmt, ...) {
	bool needLog = false;
	LoggerPtr userlogger(Logger::getLogger(module));
	switch (level) {
	case Trace:
		needLog = userlogger->isTraceEnabled();
		break;
	case Debug:
		needLog = userlogger->isDebugEnabled();
		break;
	case Info:
		needLog = userlogger->isInfoEnabled();
		break;
	case Warning:
		needLog = userlogger->isWarnEnabled();
		break;
	default:
		needLog = true;
		break;
	}

	if (needLog) {
		va_list ap;
		va_start(ap, msgfmt);
		log(userlogger, level, msgfmt, ap);
		va_end(ap);
	}
}

void sfa_log4(_sfa_in_ const wchar_t* module, _sfa_in_ LogLevel level, _sfa_in_ const wchar_t* file, _sfa_in_ const wchar_t* function,
		_sfa_in_ const int line, _sfa_in_ const wchar_t* msgfmt, ...) {
	if (NULL == file || NULL == function || NULL == msgfmt) {
		return;
	}

	bool needLog = false;
	LoggerPtr userlogger(Logger::getLogger(module));
	if (!userlogger) {
		userlogger = Logger::getRootLogger();
	}
	if (!userlogger) {
		return;
	}

	switch (level) {
	case Trace:
		needLog = userlogger->isTraceEnabled();
		break;
	case Debug:
		needLog = userlogger->isDebugEnabled();
		break;
	case Info:
		needLog = userlogger->isInfoEnabled();
		break;
	case Warning:
		needLog = userlogger->isWarnEnabled();
		break;
	default:
		needLog = true;
		break;
	}

	if (needLog) {
		va_list ap;
		va_start(ap, msgfmt);
		wchar_t acTmp[30] = { 0 };
		swprintf_s(acTmp, L"%d", line);
		std::wstring strLog;
		strLog = L"[" + std::wstring(file) + L":" + std::wstring(function)
			+ L"(" + std::wstring(acTmp) + L")] ";
		strLog += getFormatedMsg(msgfmt, ap);
		va_end(ap);
		log(userlogger, level, strLog);
	}
}
#endif

}
}
