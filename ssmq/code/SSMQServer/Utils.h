/***************************************************************************************************
  *Copyright(C): 2019, Sumscope
  *FileName    : Utils.h
  *Author      : lei.wang@sumscope.com
  *Version     : 1.0
  *Date        : 2019-05-17
  *Desc        : utils for server.
                 if have any new tool functions later, dev them here.
  *History     : Add by Lei.Wang 2019-05-17
***************************************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>


using std::string;
using std::vector;

class CUtils
{
public:
    CUtils() {}
    ~CUtils() {}

    //����process id
    static string parseProcessID(const string &sBindKey, const string &sp);

    //����topic id
    static string parseTopicID(const string &inTpoic,  const string &sp);

    //�ַ����ָ�
    static void split(const string& strLine, const char cSplit, vector<string>& vecContent);

    //�ж�des�ַ������Ƿ����sub�Ӵ�
    static bool isInclude(const string &des, const string &sub);
};

#endif /* UTILS_H */
