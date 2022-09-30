/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : sfa_utils.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-08-19
  *Desc        : utils for server.
                 if have any new tool functions later, dev them here.
  *History     : Add by Lei.Wang 2019-08-19
***************************************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <sfa/sfa_macro.h>
#include <string>
#include <vector>

using std::string;
using std::vector;


namespace sfa {
namespace utils {

class CUtils {
public:
	CUtils() {}
	~CUtils() {}

	//trim
	static string trim(_sfa_in_ const string &src);

	//split the string to vector
	static void split(_sfa_in_ const string& strLine, _sfa_in_ const char cSplit, _sfa_out_ vector<string>& vecContent);

	//determine whether the des string contains a sub string
	static bool isInclude(_sfa_in_ const string &des, _sfa_in_ const string &sub);
};

}
}

#endif /* UTILS_H */
