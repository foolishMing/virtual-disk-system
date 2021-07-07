#include "StringTool.h"
#include "Log.h"

void StringTools::StringSplitBySpace(const string_local& in, std::vector<string_local>& out)
{
	if (in.empty())
	{
		Log::LogWarn(L"不合法的操作：试图对空串进行Split()操作");
		return;
	}
	string_stream_local str(in);
	string_local buf;
	while (str >> buf) {
		out.push_back(buf);
	}
}

/*
-constraints:
1、以空格为分隔符
2、引号中的空格当作普通字符处理
*/
bool StringTools::StringSplitBySpaceWithQuotes(const string_local& in, std::vector<string_local>& out)
{
	if (in.empty())
	{
		Log::LogWarn(L"不合法的操作：试图对空串进行Split()操作");
		return false;
	}
	bool is_quote = false;	
	string_local buffer = {};
	for (auto item : in)
	{
		if (item == CharSet::char_space) {//遇到空格
			if (is_quote) //在引号中，空格当作普通字符处理
			{	
				buffer += item;
				continue;
			}
			if(!buffer.empty()) out.push_back(buffer);//忽略连续的空格
			buffer = {};
		}
		else if (item == CharSet::char_doublequote)//遇到引号，改变引号状态
		{
			is_quote = !is_quote;
			buffer += item ;
		}
		else
		{
			buffer += item;
		}
	}
	//清空缓冲区
	if (!buffer.empty())
	{
		out.push_back(buffer);
	}
	//存在未匹配的单引号，不合法的输入
	if (is_quote == true) { 
		return false;
	}
	return true;
}


string_local StringTools::StringTrimed(const string_local& in)
{
	string_local out = in;
	if (out.empty())
	{
		Log::LogWarn(L"不合法的操作：试图对空串进行Trimed()操作");
		return out;
	}
	out.erase(0, out.find_first_not_of(L" "));
	out.erase(out.find_last_not_of(L" ") + 1);
	return out;
}

string_local StringTools::ToLowercase(const string_local& s)
{
	string_local ret = s;
	for (auto item : s)
	{
		if (item >= 'A' && item <= 'Z')
		{
			item = item - 'A' + 'a';
		}
	}
	return ret;
}

bool StringTools::HasUppercase(const string_local& s)
{
	for (auto item : s)
	{
		if (item >= 'A' && item <= 'Z')
		{
			return true;
		}
	}
	return false;
}


bool StringTools::IsEqual(const string_local& lhs, const string_local& rhs)
{
	auto lc_lhs = ToLowercase(lhs);
	auto lc_rhs = ToLowercase(rhs);
	if (lc_lhs == lc_rhs)
	{
		return true;
	}
	return false;
}

//约束：in的长度不能为0
string_local StringTools::GetStringSuffix(const string_local& in, size_t cnt)
{
	assert(!in.empty());
	if (in.empty())
	{
		Log::LogWarn(L"不合法的操作：试图获取空串的后缀子串");
		return L"";
	}
	int max_len = static_cast<int>(std::min(in.length(), cnt));
	return in.substr(in.size() - cnt, cnt);
}

//String严格匹配（忽略大小写）
bool StringTools::IsStringStrictEqualTo(const string_local& lhs, const string_local& rhs)
{
	auto lc_lhs = ToLowercase(lhs);
	auto lc_rhs = ToLowercase(rhs);
	return (lc_lhs == lc_rhs);
}

//String模糊匹配
bool StringTools::IsStringFuzzyEqualTo(const string_local& text, const string_local& pattern)
{
	auto lc_text = ToLowercase(text);
	auto lc_pattern = ToLowercase(pattern);
	if (IsFuzzyMatch(lc_text.c_str(), lc_pattern.c_str()))
	{
		return true;
	}
	return IsStringStrictEqualTo(text, pattern);
}

//CString模糊匹配，支持通配符"*"和"?"
bool StringTools::IsFuzzyMatch(const char_local* str, const char_local* pattern)
{
	if (*pattern == L'\0')
	{
		return true;
	}
	if (*pattern == L'*')
	{
		while (*str != L'\0')
		{
			if (IsFuzzyMatch((const char_local*)str++, (const char_local*)pattern + 1))
			{
				return true;
			}
		}
	}
	if (*str == L'\0')
	{
		return false;
	}
	if (*str == *pattern || *pattern == L'?')
	{
		return IsFuzzyMatch(str + 1, pattern + 1);
	}
	if (*str != *pattern)
	{
		return false;
	}
	return false;
}

void StringTools::StringDerefDoubleQuote(string_local& s)
{
	if (s.length() < 2)
	{
		return;
	}
	if (s[0] == CharSet::char_doublequote && s.back() == CharSet::char_doublequote)
	{
		s = s.substr(1, s.length() - 2);
	}
}
