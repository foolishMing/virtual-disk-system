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
//option : 命令行选项
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


//命令选项开关
struct OptionSwitch
{
	bool _s = false;
	bool _ad = false;
	bool _y = false;
	bool _d = false;
};

//返回类型
enum ReturnType
{

};

//目录相关信息
struct DirInfo
{
	int32_t file_cnt; //文件数
	int32_t dir_cnt;  //目录数
	size_t  tot_size; //总大小
};

//错误提示
namespace ErrorTips {
	//不是内部或外部命令,也不是可运行的程序
	extern const string_local gsCommandIsNotFound;	

	//命令语法不正确
	extern const string_local gsCommandIsIllegal;	

	//参数格式不正确
	extern const string_local gsOptionsIsIllegal;

	//无效的开关
	extern const string_local gsOptionsIsInvalid;

	//文件名、目录名或卷标语法不正确
	extern const string_local gsTokenNameIsIllegal; 

	//不合法的磁盘路径名称
	extern const string_local gsDiskPathIsIllegal; 
	
	//系统找不到指定的磁盘路径
	extern const string_local gsDiskPathIsNotFound; 

	//磁盘路径已存在
	extern const string_local gsDiskPathIsExist; 

	//不合法的虚拟磁盘路径名称
	extern const string_local gsMemoryPathIsIllegal; 

	//系统找不到指定的虚拟磁盘路径
	extern const string_local gsMemoryPathIsNotFound; 

	//系统找不到指定的虚拟磁盘文件
	extern const string_local gsMemoryFileIsNotFound;

	//虚拟磁盘中存在一个重名文件，或找不到文件
	extern const string_local gsMemoryFileIsExist;

	//虚拟磁盘路径已存在
	extern const string_local gsMemoryPathIsExist;

	//虚拟磁盘路径拒绝访问
	extern const string_local gsMemoryPathAccessDenied;
}

//字符集
namespace CharSet
{
	//空格
	extern const char_local char_space;

	//双引号
	extern const char_local char_doublequote;

	//斜杠
	extern const char_local char_slash;

	//反斜杠
	extern const char_local char_backslash;

	//冒号
	extern const char_local char_colon;

	//星号
	extern const char_local char_asterisk;

	//问号
	extern const char_local char_question;

	//或
	extern const char_local char_or;

	//小于号
	extern const char_local char_lessthan;

	//大于号
	extern const char_local char_morethan;
}

//全局常量
namespace Constant
{
	//当前目录
	extern const string_local gs_cur_dir_token;
	
	//上级目录
	extern const string_local gs_parent_dir_token;

	// /ad
	extern const string_local gs_option_ad;

	// /d
	extern const string_local gs_option_d;

	// /s
	extern const string_local gs_option_s;
	
	// /y
	extern const string_local gs_option_y;
}

#endif // !__DEFINE_H__
