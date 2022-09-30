#include <algorithm>
#include "sfa_utils.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace sfa {
namespace utils {

string CUtils::trim(_sfa_in_ const string &src) {
	string ret;
	if (src.empty()) {
		return ret;
	}
	ret = src;
	ret.erase(0, ret.find_first_not_of(" "));
	ret.erase(ret.find_last_not_of(" ") + 1);
	return ret;
}

void CUtils::split(_sfa_in_ const string& strLine, _sfa_in_ const char cSplit, _sfa_out_ vector<string>& vecContent) {
	//single field
	if (strLine.find_first_of(cSplit, 0) == string::npos) {
		vecContent.push_back(strLine);
	}
	//multi fields
	else {
		//get the fields
		string::size_type lastSplit = strLine.find_first_of(cSplit, 0);
		vecContent.push_back(strLine.substr(0, lastSplit));

		string::size_type curSplit = strLine.find_first_of(cSplit, lastSplit + 1);
		while (curSplit != string::npos) {
			vecContent.push_back(strLine.substr(lastSplit + 1, curSplit - lastSplit - 1));
			lastSplit = curSplit;
			curSplit = strLine.find_first_of(cSplit, lastSplit + 1);
		}

		//get the last field
		string strLastValue = strLine.substr(lastSplit + 1, strLine.size() - lastSplit);
		vecContent.push_back(strLastValue);
	}
}

bool CUtils::isInclude(_sfa_in_ const string &des, _sfa_in_ const string &sub) {
	string::size_type idx = des.find(sub);
	if (string::npos != idx) { return true; }
	return false;
}

}
}
