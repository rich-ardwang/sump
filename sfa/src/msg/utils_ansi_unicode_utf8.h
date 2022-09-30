#pragma once
#include <string>
#include <codecvt>

namespace sfa {
namespace msg {
namespace utils {

#ifdef _MSC_VER
	const char* GBK_LOCALE_NAME = ".936";
#else
	const char* GBK_LOCALE_NAME = "zh_CN.GBK";
#endif

static std::wstring AnsiToUnicode(const std::string& str){
	std::wstring wstr;
	std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> wcv
		(new std::codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));

	try{ wstr = wcv.from_bytes(str); }catch (...){}
	return wstr;
}

static std::string UnicodeToAnsi(const std::wstring& wstr){
	std::string str;
	std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> wcv
		(new std::codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));

	try{ str = wcv.to_bytes(wstr); }catch (...){}
	return str;
}

static std::wstring Utf8ToUnicode(const std::string& str){
	std::wstring wstr;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;

	try{ wstr = wcv.from_bytes(str); }catch (...){}
	return wstr;
}

static std::string UnicodeToUtf8(const std::wstring& wstr){
	std::string str;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;

	try{ str = wcv.to_bytes(wstr); }catch (...){}
	return str;
}

static std::string AnsiToUtf8(const std::string& str) {
	std::wstring wstr = AnsiToUnicode(str);
	std::string utf8 = UnicodeToUtf8(wstr);
	return utf8;
}

static std::string Utf8ToAnsi(const std::string& str) {
	std::wstring wstr = Utf8ToUnicode(str);
	std::string ansi = UnicodeToAnsi(wstr);
	return ansi;
}

}
}
}