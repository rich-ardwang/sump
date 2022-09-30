// boostTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <Windows.h>


using namespace std;

string parseProcessID(const string &sBindKey, const string &sp) {
    string sRet;
    int pos = 0;
    int len = sBindKey.length();
    if (string::npos != (pos = sBindKey.find_last_of(sp, len))) {
        sRet = sBindKey.substr(pos+1, len);
    }
    return sRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
    string pid;
    //pid = parseProcessID("ceda.event.11688", ".");
    pid = parseProcessID("ceda.mng.7786", ".");
    cout << pid << endl;
    system("pause");
	return 0;
}

