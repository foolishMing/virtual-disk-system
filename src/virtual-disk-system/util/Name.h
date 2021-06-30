#pragma once
#include "Common.h"

class Name {
public:
	Name(std::wstring name);
	~Name();
	//�ж��Ƿ�ͬ������ȷƥ�䣩,��Сд������
	bool isStrictEqualTo(const Name& rhs) const;
	//�ж��Ƿ�ͬ����ģ��ƥ�䣩����Сд������
	bool isFuzzyEqualTo(const Name& rhs) const;
	std::wstring getName() const;
	//�ַ���ģ��ƥ�䣬֧��ͨ���"*"��"?"
    bool IsFuzzyMatch(const wchar_t* str, const wchar_t* pattern) const;

private:
	std::wstring m_name = L"";
	std::wstring m_internal_name = L"";
	std::wstring toLowercase(const std::wstring& str) const;
};

