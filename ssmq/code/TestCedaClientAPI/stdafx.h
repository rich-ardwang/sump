// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <iostream>
#include <windows.h>
#include "CedaApi.h"
#include "Config.h"

class CCedaObserve : public CedaClient
{
public:
    void onMessage(const char* topic, const char* content, int len);

    void onEvent(int eventID);

};

// TODO: 在此处引用程序需要的其他头文件
