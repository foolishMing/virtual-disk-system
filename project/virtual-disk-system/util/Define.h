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

//名词/缩写解释
//gs : global string，全局字符串常量
//token : 路径上语法正确的目录、文件、卷名称

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


//错误提示
namespace Tips {
	extern const string_local gsCommandIsNotFound;	//不是内部或外部命令,也不是可运行的程序
	extern const string_local gsCommandIsIllegal;	//命令语法不正确

	extern const string_local gsTokenNameIsIllegal; //文件名、目录名或卷标语法不正确

	extern const string_local gsDiskPathIsIllegal; //不合法的磁盘路径名称
	extern const string_local gsDiskPathIsNotFound; //系统找不到指定的磁盘路径
	extern const string_local gsDiskPathIsExist; //磁盘路径已存在

	extern const string_local gsMemoryPathIsIllegal; //不合法的虚拟磁盘路径名称
	extern const string_local gsMemoryPathIsNotFound; //系统找不到指定的虚拟磁盘路径
	extern const string_local gsMemoryPathIsExist; //虚拟磁盘路径已存在
}

//字符集
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

//全局常量
namespace Constant
{
	extern const string_local gs_cur_dir_token;//当前目录
	extern const string_local gs_parent_dir_token;//上级目录
}

#endif // !__DEFINE_H__
