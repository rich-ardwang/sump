//-------------------------------------------------------------------------------------
//--Copyright	:	Sumscope
//--FileName	:	BaseStringFunc.h
//--Author		:	cheng.xu@sumscope.com
//--Date		:	2015.01.29
//--Desc		:	Multi-Byte/Unicode֧��,string�ĳ��ú���
//--History		:	Created by cheng.xu, 2015.01.29
//--				ȥ��MFC����,MFC�����ķ�����ȡ��BaseStringFunc_MFC.h 2015.02.02
//-------------------------------------------------------------------------------------

#pragma once
#include "BaseStringType.h"
#include "Windows.h"

namespace SS_BASE
{
    /************************************************************************
    * WideCharToMultiByte�Ĵ���ҳ�����������ת�����ַ�����صĴ���ҳ��
    * MultiByteToWideChar�Ĵ���ҳ���������һ�����ֽ��ַ�����صĴ���ҳ��
    * ���õĴ���ҳ��CP_ACP��CP_UTF8������
    * ʹ��CP_ACP����ҳ��ʵ����ANSI��Unicode֮���ת����
    * ʹ��CP_UTF8����ҳ��ʵ����UTF-8��Unicode֮���ת����
    ************************************************************************/

    //ANSI to Unicode
	static std::wstring AnsiToUnicode(const std::string& str)
    {
        int  unicodeLen = ::MultiByteToWideChar(/*CP_ACP*/936, 0, str.c_str(), -1, NULL, 0);
        if (unicodeLen <= 0)
        {
            return L"";
        }

        wchar_t* pUnicode = new wchar_t[unicodeLen+1];
        memset(pUnicode, 0, (unicodeLen+1)*sizeof(wchar_t));

        ::MultiByteToWideChar(/*CP_ACP*/936, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);

        std::wstring rt = pUnicode;

        delete[] pUnicode;
        pUnicode = NULL;

        return rt;
    }

    //Unicode to ANSI
    static std::string UnicodeToAnsi(const std::wstring& str)
    {
        int iTextLen = WideCharToMultiByte(/*CP_ACP*/936, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
        if (iTextLen <= 0)
        {
            return "";
        }

        char* pElementText = new char[iTextLen + 1];
        memset(pElementText, 0, sizeof(char)*(iTextLen + 1));

        ::WideCharToMultiByte(/*CP_ACP*/936, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);

        std::string strText = pElementText;

        delete[] pElementText;
        pElementText = NULL;

        return strText;
    }

    //UTF-8 to Unicode
    static std::wstring UTF8ToUnicode(const std::string& str)
    {
        int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        if (unicodeLen <= 0)
        {
            return L"";
        }

        wchar_t* pUnicode = new  wchar_t[unicodeLen+1];
        memset(pUnicode, 0, (unicodeLen+1)*sizeof(wchar_t));

        ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);

        std::wstring  rt = pUnicode;

        delete[] pUnicode;
        pUnicode = NULL;

        return rt;  
    }

    //Unicode to UTF-8
    static std::string UnicodeToUTF8(const std::wstring& str )
    {
        int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
        if (iTextLen <= 0)
        {
            return "";
        }

        char* pElementText = new char[iTextLen + 1];
        memset(pElementText, 0, sizeof(char) * (iTextLen + 1));
        ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);

        std::string strText = pElementText;

        delete[] pElementText;
        pElementText = NULL;

        return strText;
    }

    //UTF-8 to GBK
    static t_string Utf8ToGBK(const std::string& strUtf8)
    {
#ifdef UNICODE
        return UTF8ToUnicode(strUtf8);
#else
		std::wstring strUnicode = UTF8ToUnicode(strUtf8);
        return UnicodeToAnsi(strUnicode);
#endif
    }

	//UTF-8 to GBK(Ansi)
	static std::string Utf8ToGBKA(const std::string& strUtf8)
	{
		std::wstring strUnicode = UTF8ToUnicode(strUtf8);
		return UnicodeToAnsi(strUnicode);
	}


    //GBK to UTF-8
    static std::string GBK2Utf8(const t_string& strGBK)
    {
#ifdef UNICODE
        return UnicodeToUTF8(strGBK);
#else
        std::wstring strUnicode = AnsiToUnicode(strGBK);
        return UnicodeToUTF8(strUnicode);
#endif
    }

	//GBK(Ansi) to UTF-8
	static std::string GBK2Utf8A(const std::string& strGBK)
	{
		std::wstring strUnicode = AnsiToUnicode(strGBK);
		return UnicodeToUTF8(strUnicode);
	}

	//t_string to wstring
	static std::wstring t_string2wstring(const t_string& str)
	{
#ifdef UNICODE
		std::wstring wstr(str);
		return wstr;
#else
		std::string tempStr(str);
		return AnsiToUnicode(tempStr);
#endif
	}

	//char to t_string
    static t_string char2t_string(const char* buf)
    {
        if (NULL == buf)
        {
            return _T("");
        }

#ifdef UNICODE
        return AnsiToUnicode(buf);
#else
        return buf;
#endif
    }

	//t_string to string
    static std::string t_string2string(t_string buf)
    {
#ifdef UNICODE
        return UnicodeToAnsi(buf);
#else
        return buf;
#endif
    }

	//t_string Replace ģ��CString��Replace
	static int replace_all(t_string& str,  const t_string& pattern,  const t_string& newpat) 
	{ 
		int count = 0; 
		const size_t nsize = newpat.size(); 
		const size_t psize = pattern.size(); 

		for(size_t pos = str.find(pattern, 0);  
			pos != t_string::npos; 
			pos = str.find(pattern,pos + nsize)) 
		{ 
			str.replace(pos, psize, newpat); 
			count++; 
		} 

		return count; 
	}

	//t_string CompareNoCase ģ��CString��CompareNoCase
	static int CompareNoCase(const t_string& x, const t_string& y)
	{
		t_string::const_iterator p = x.begin();
		t_string::const_iterator q = y.begin();
		while (p != x.end() && q != y.end() && toupper(*p) == toupper(*q))
		{
			++p;
			++q;
		}
		if (p == x.end() && q == y.end())
		{
			return 0;
		}
		if (p != x.end() && q == y.end())
		{
			return toupper(*p);
		}
		if (p == x.end() && q != y.end())
		{
			return -toupper(*q);
		}
		return (toupper(*p) - toupper(*q));
	}
}
