// commonTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>

using namespace std;

/*
string sha256(const string &str)
{
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}
*/

int _tmain(int argc, _TCHAR* argv[])
{
    string st = "中国";
    cout << st.length() << endl;

    system("pause");
	return 0;
}

