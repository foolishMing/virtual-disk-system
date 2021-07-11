#pragma once 

#ifndef __STRINGTOOL_H__
#define __STRINGTOOL_H__
#include "Define.h"
#include "Log.h"

/*
unit test：
StringSplitBySpaceWithQuotes(); //字符串分割(基于空格)
IsStringFuzzyEqualTo();	//模糊匹配
*/

class StringTools
{
public:
	//字符串分割（基于空格）
	static void StringSplitBySpace(const string_local& in, std::vector<string_local>& out);
	//字符串分割（基于空格），但不处理被引号包裹的空格
	static bool StringSplitBySpaceAndQuotes(const string_local& in, std::vector<string_local>& out);
	//去除首尾空格
	static string_local Trimed(const string_local& s);
	//去除首尾引号
	static void StringDerefDoubleQuote(string_local& s);

	//去除匹配的双引号
	static bool FilterMatchedDoubleQuotes(const string_local& src, string_local& dst);

	//判断字符串是否相等
	static bool IsEqual(const string_local& lhs, const string_local& rhs);

	//判断字符串中是否有大写字母
	static bool HasUppercase(const string_local& s);
	//获取长度为size的后缀子串
	static string_local GetStringSuffix(const string_local& in, size_t cnt);

	//String严格匹配（忽略大小写）
	static bool IsStringStrictEqualTo(const string_local& lhs, const string_local& rhs);
	//String模糊匹配
	static bool IsStringFuzzyEqualTo(const string_local& text, const string_local& pattern);
	//CString模糊匹配，支持通配符"*"和"?"
	static bool IsFuzzyMatch(const char_local* str, const char_local* pattern);

	//判断是否存在通配符
	static bool HasWildcard(const std::vector<string_local>& tokens);

	//秒时间戳转字符串
	static string_local TimeStampToDateTimeString(time_t ts);

	//数字转格式化的字符串
	static string_local FormatFromNumber(int num);

private:
	//字符串转小写
	static string_local ToLowercase(const string_local& s);
};

#endif // !__STRINGTOOL_H__
