// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
