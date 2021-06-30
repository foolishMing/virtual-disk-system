#pragma once
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <map>
#define __struct__ struct
#define interface __struct__

//�ڵ�����
enum NodeType {
	Directory,
	File
};




//�ַ���ģ��ƥ�䣬֧��ͨ���"*"��"?"
static bool IsFuzzyMatch(const char* str, const char* pattern)
{
	if (*pattern == '\0') 
	{
		return true;
	}
	if (*pattern == '*') 
	{
		while (*str != '\0') 
		{
			if (IsFuzzyMatch(str++, pattern + 1)) 
			{
				return true;
			}
		}
	}
	if (*str == '\0')
	{
		return false;
	}
	if (*str == *pattern || *pattern == '?')
		return IsFuzzyMatch(str + 1, pattern + 1);
	if (*str != *pattern)return false;
}

//�ڵ�����
class Name {
public:
	Name(std::string name) 
	{
		m_name = name;
	}
	~Name() {}
	//�ж��Ƿ�ͬ��
	bool isEqualTo(const Name& rhs)const {
		if (IsFuzzyMatch(m_name.c_str(), rhs.getName().c_str())) {
			return true;
		}
		return (m_name == rhs.getName());
	}
	
	std::string getName() const
	{
		return m_name;
	}

private:
	std::string m_name = "";
};







