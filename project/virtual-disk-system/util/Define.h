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


//元对象，所有对象都必须继承
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
	extern const string_local gsILLegalCommand;	//命令语法不正确
	extern const string_local gsIllegalDiskPath; //系统找不到磁盘路径
	extern const string_local gsIllegalMemoryPath; //系统找不到虚拟磁盘路径
}



#endif // !__DEFINE_H__
