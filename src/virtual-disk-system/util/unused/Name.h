#pragma once
#include "Common.h"

class Name {
public:
	Name();
	Name(string_local& name);
	~Name();
	string_local getName() const;
	bool isStrictEqualTo(const Name& rhs) const;//�ж��Ƿ�ͬ������ȷƥ�䣩,��Сд������
	bool isFuzzyEqualTo(const Name& rhs) const;//�ж��Ƿ�ͬ����ģ��ƥ�䣩����Сд������
    bool IsFuzzyMatch(const char_local* str, const char_local* pattern) const;//�ַ���ģ��ƥ�䣬֧��ͨ���"*"��"?"

private:
	string_local m_name = L"";
	string_local m_internal_name = L"";
	string_local toLowercase(const string_local& str) const;
};

