#include "Name.h"

Name::Name() 
{

}

Name::Name(string_local& name)
{
	m_name = name;
	m_internal_name = toLowercase(m_name);
}

Name::~Name()
{

}

bool Name::isStrictEqualTo(const Name& rhs) const
{
	string_local pattern_string = toLowercase(rhs.getName());
	return (m_internal_name == pattern_string);
}

bool Name::isFuzzyEqualTo(const Name& rhs) const
{
	string_local pattern_string = toLowercase(rhs.getName());
	if (IsFuzzyMatch(m_name.c_str(), pattern_string.c_str()))
	{
		return true;
	}
	return isStrictEqualTo(rhs);
}

string_local Name::getName() const
{
	return m_name;
}

string_local Name::toLowercase(const string_local& str) const 
{
	string_local ret;
	ret.assign(str);
	std::transform(ret.begin(), ret.end(), ret.begin(), tolower);
	return ret;
}

//字符串模糊匹配，支持通配符"*"和"?"
bool Name::IsFuzzyMatch(const char_local* str, const char_local* pattern) const
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