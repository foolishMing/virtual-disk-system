#pragma once
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <map>
#define __struct__ struct
#define interface __struct__

//节点类型
enum NodeType {
	Directory,
	File
};




//字符串模糊匹配，支持通配符"*"和"?"
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

//节点名称
class Name {
public:
	Name(std::string name) 
	{
		m_name = name;
	}
	~Name() {}
	//判断是否同名
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







