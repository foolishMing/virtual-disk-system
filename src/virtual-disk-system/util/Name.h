#pragma once
#include "Common.h"

class Name {
public:
	Name();
	Name(string_local& name);
	~Name();
	string_local getName() const;
	bool isStrictEqualTo(const Name& rhs) const;//判断是否同名（精确匹配）,大小写不敏感
	bool isFuzzyEqualTo(const Name& rhs) const;//判断是否同名（模糊匹配），大小写不敏感
    bool IsFuzzyMatch(const char_local* str, const char_local* pattern) const;//字符串模糊匹配，支持通配符"*"和"?"

private:
	string_local m_name = L"";
	string_local m_internal_name = L"";
	string_local toLowercase(const string_local& str) const;
};

