#include "Name.h"

Name::Name(std::wstring name)
{
	m_name = name;
	m_internal_name = toLowercase(m_name);
}

Name::~Name()
{

}

bool Name::isStrictEqualTo(const Name& rhs) const
{
	std::wstring pattern_string = toLowercase(rhs.getName());
	return (m_internal_name == pattern_string);
}

bool Name::isFuzzyEqualTo(const Name& rhs) const
{
	std::wstring pattern_string = toLowercase(rhs.getName());
	if (IsFuzzyMatch(m_name.c_str(), pattern_string.c_str()))
	{
		return true;
	}
	return isStrictEqualTo(rhs);
}

std::wstring Name::getName() const
{
	return m_name;
}

std::wstring Name::toLowercase(const std::wstring& str) const 
{
	std::wstring ret;
	ret.assign(str);
	std::transform(ret.begin(), ret.end(), ret.begin(), tolower);
	return ret;
}

//字符串模糊匹配，支持通配符"*"和"?"
bool Name::IsFuzzyMatch(const wchar_t* str, const wchar_t* pattern) const
{
	if (*pattern == L'\0')
	{
		return true;
	}
	if (*pattern == L'*')
	{
		while (*str != L'\0')
		{
			if (IsFuzzyMatch((const wchar_t*)str++, (const wchar_t*)pattern + 1))
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
}