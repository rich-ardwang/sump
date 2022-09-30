/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : Config.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-08-12
  *Desc        : DDS API global config
  *History     : Add by Lei.Wang 2019-08-12
***************************************************************************************************/
#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <string>
#ifdef WIN32
#include <Windows.h>
#include <stdio.h>
#else
#define  MAX_PATH 260
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#endif

#define CONF_FILE_PATH		"dds.ini"
#define GET_GOL_CONF		CConfig::getInstance()->GetGlobalConf()


/* DDS API global config */
typedef struct tagConf {
	std::string		s_smaple;
	int				n_smaple;
} gConf;

class CConfig {
public:
	static CConfig *getInstance() {
		static CConfig inst_;
		return &inst_;
	}

public:
	gConf GetGlobalConf();

private:
	CConfig();
	CConfig(std::string path);
	CConfig(const CConfig &);
	CConfig &operator=(const CConfig &);
	void loadGlobalConfig();

	//read char value from the ini file
	char *GetIniKeyString(const char *title, const char *key, const char *filename);
	//read int value from the ini file
	int GetIniKeyInt(const char *title, const char *key, const char *filename);

private:
	std::string		conf_file_;		//config file path and name
	gConf			global_conf_;	//global config content
};

#endif //__CONFIG_H__ endif