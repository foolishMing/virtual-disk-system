#pragma once
#include "Common.h"

class Name {
public:
	Name(std::wstring name);
	~Name();
	//判断是否同名（精确匹配）,大小写不敏感
	bool isStrictEqualTo(const Name& rhs) const;
	//判断是否同名（模糊匹配），大小写不敏感
	bool isFuzzyEqualTo(const Name& rhs) const;
	std::wstring getName() const;
	//字符串模糊匹配，支持通配符"*"和"?"
    bool IsFuzzyMatch(const wchar_t* str, const wchar_t* pattern) const;

private:
	std::wstring m_name = L"";
	std::wstring m_internal_name = L"";
	std::wstring toLowercase(const std::wstring& str) const;
};

