#include "StdAfx.h"
#include "Utils.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////
string CUtils::parseProcessID(const string &sBindKey, const string &sp) {
    std::string sRet;
    int pos = 0;
    int len = sBindKey.length();
    if (std::string::npos != (pos = sBindKey.find_last_of(sp, len))) {
        sRet = sBindKey.substr(pos+1, len);
    }
    return sRet;
}

string CUtils::parseTopicID(const string &inTpoic, const string &sp)
{
    std::string sRet;
    int pos = 0;
    if (std::string::npos != (pos = inTpoic.find_first_of(sp, 0))) {
        sRet = inTpoic.substr(0, pos);
    }
    else {
        sRet = inTpoic;
    }
    return sRet;
}

void CUtils::split(const string& strLine, const char cSplit, vector<string>& vecContent) {
    //单字段
    if (strLine.find_first_of(cSplit, 0) == string::npos) {
        vecContent.push_back(strLine);
    }
    //多字段
    else {
        //遍历取出字段
        size_t lastSplit = strLine.find_first_of(cSplit, 0);
        vecContent.push_back(strLine.substr(0, lastSplit));

        size_t curSplit = strLine.find_first_of(cSplit, lastSplit + 1);
        while(curSplit != string::npos) {
            vecContent.push_back(strLine.substr(lastSplit + 1, curSplit - lastSplit - 1));
            lastSplit = curSplit;
            curSplit = strLine.find_first_of(cSplit, lastSplit + 1);
        }

        //取出最后一个字段
        string strLastValue = strLine.substr(lastSplit + 1, strLine.size() - lastSplit);
        vecContent.push_back(strLastValue);
    }
}

bool CUtils::isInclude(const string &des, const string &sub) {
    string::size_type idx = des.find(sub);
    if (string::npos != idx) { return true; }
    return false;
}