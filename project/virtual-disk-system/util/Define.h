#pragma once
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifndef interface
#define __struct__ struct
#define interface __struct__
#endif 


//元对象
interface Object
{
	virtual void Create() = 0;
	virtual void Destroy() = 0;
};


//定义本地字符集
typedef std::wstring string_local;
typedef wchar_t char_local;
typedef std::wistringstream string_stream_local;


//全局常量
namespace Tips {
	extern const string_local gsUndefinedCommand;	//不是内部或外部命令,也不是可运行的程序
	extern const string_local gsIllegalCommand;	//命令语法不正确
	extern const string_local gsIllegalDiskPath; //系统找不到磁盘路径
	extern const string_local gsIllegalMemoryPath; //系统找不到虚拟磁盘路径
	extern const string_local gsIllegalName;
}

namespace CharSet
{
	extern const char_local char_space;//空格
	extern const char_local char_doublequote;//双引号
	extern const char_local char_slash;//斜杠
	extern const char_local char_backslash;//反斜杠
	extern const char_local char_colon;//冒号
	extern const char_local char_asterisk;//星号
	extern const char_local char_question;//问号
	extern const char_local char_or;//或
	extern const char_local char_lessthan;//小于号
	extern const char_local char_morethan;//大于号
}

#endif // !__DEFINE_H__
