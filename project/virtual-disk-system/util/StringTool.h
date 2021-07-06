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
	static bool StringSplitBySpaceWithQuotes(const string_local& in, std::vector<string_local>& out);
	//去除首尾空格
	static string_local StringTrimed(const string_local& s);
	//判断字符串是否相等
	static bool IsEqual(const string_local& lhs, const string_local& rhs);
	//字符串转小写
	static string_local ToLowercase(const string_local& s);
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
};

#endif // !__STRINGTOOL_H__
