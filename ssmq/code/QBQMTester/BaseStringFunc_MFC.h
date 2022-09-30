//-------------------------------------------------------------------------------------
//--Copyright	:	Sumscope
//--FileName	:	BaseStringFunc_MFC.h
//--Author		:	cheng.xu@sumscope.com
//--Date		:	2015.02.02
//--Desc		:	BaseStringFunc的基础上增加依赖MFC时常用的方法
//--History		:	Created by cheng.xu, 2015.01.29
//--				
//-------------------------------------------------------------------------------------

#pragma once
#include "BaseStringFunc.h"
#include <afxwin.h>

namespace SS_BASE
{
	//CString to wstring
	static std::wstring CString2wstring(const CString& str)
	{
#ifdef UNICODE
		std::wstring wstr(str);
		return wstr;
#else
		string tempStr(str);
		return AnsiToUnicode(tempStr);
#endif
	}

	//char to CString
    static CString char2CString(const char* buf)
    {
        if (NULL == buf)
        {
            return _T("");
        }

#ifdef UNICODE
        return AnsiToUnicode(buf).c_str();
#else
        return buf;
#endif
    }

	//CString to string
    static std::string CString2string(const CString& str)
    {
#ifdef UNICODE
        if (str.IsEmpty())
        {
            return "";
        }

        CString strTemp = str;
        std::string sAnsi = UnicodeToAnsi(strTemp.GetBuffer());
        strTemp.ReleaseBuffer();

        return sAnsi;
#else
        return (string)str;
#endif
    }
}
