#include "stdafx.h"
#include "Config.h"

std::string getModulePath() {
	char buf[1024] = { 0 };
	if (::GetModuleFileNameA(NULL, buf, 1024) == 0) {
		return "";
	}
	if (::GetLongPathNameA(buf, buf, 1024) == 0) {
		return "";
	}

	char* end = strrchr(buf, '\\');
	buf[end - buf] = 0;
	std::string path = buf;
	std::string name = (end + 1);
	return path;
}

gConf CConfig::GetGlobalConf() {
	return global_conf_;
}

CConfig::CConfig() {
#ifdef WIN32
	std::string module_path  = getModulePath();
	if (!module_path.empty()) {
		conf_file_ = module_path + "\\" + CONF_FILE_PATH;
	}
	else {
		conf_file_ += ".\\";
		conf_file_ += CONF_FILE_PATH;
	}
#else
	conf_file_ = CONF_FILE_PATH;
#endif
	loadGlobalConfig();
}

CConfig::CConfig(std::string path) : conf_file_(path) {
	loadGlobalConfig();
}

void CConfig::loadGlobalConfig() {
	global_conf_.s_smaple = GetIniKeyString("maincfg", "s_sple", conf_file_.c_str());
	global_conf_.n_smaple = GetIniKeyInt("maincfg", "n_sple", conf_file_.c_str());
}

//从INI文件读取字符串类型数据
char *CConfig::GetIniKeyString(const char *title, const char *key, const char *filename) {
	FILE *fp;
	char szLine[1024];
	static char tmpstr[1024];
	int rtnval;
	int i = 0;
	int flag = 0;
	char *tmp;

	fopen_s(&fp, filename, "r");
	if (fp == NULL) {
		printf("have no such file!\n");
		return "";
	}
	while (!feof(fp)) {
		rtnval = fgetc(fp);
		if (rtnval == EOF) {
			break;
		}
		else {
			szLine[i++] = rtnval;
		}
		if (rtnval == '\n') {
#ifndef WIN32
			i--;
#endif	
			szLine[--i] = '\0';
			i = 0;
			tmp = strchr(szLine, '=');

			if ((tmp != NULL) && (flag == 1)) {
				if (strstr(szLine, key) != NULL) {
					//注释行
					if ('#' == szLine[0]) {}
					else if ('/' == szLine[0] && '/' == szLine[1]) {}
					else {
						//找打key对应变量
						strcpy_s(tmpstr, tmp + 1);
						fclose(fp);
						return tmpstr;
					}
				}
			}
			else {
				strcpy_s(tmpstr, "[");
				strcat_s(tmpstr, title);
				strcat_s(tmpstr, "]");
				if (strncmp(tmpstr, szLine, strlen(tmpstr)) == 0) {
					//找到title
					flag = 1;
				}
			}
		}
	}
	fclose(fp);
	return "";
}

int CConfig::GetIniKeyInt(const char *title, const char *key, const char *filename) {
	return atoi(GetIniKeyString(title, key, filename));
}