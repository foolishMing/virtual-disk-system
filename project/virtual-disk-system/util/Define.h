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

#ifndef _interface_
#define __struct__ struct
#define _interface_ __struct__
#endif 

//名词/缩写解释
//gs : global string，全局字符串常量
//option : 命令行选项
//token : 路径上语法正确的目录、文件、卷名称

//元对象
_interface_ Object
{
	virtual void Create() = 0;
	virtual void Destroy() = 0;
};


//定义本地字符集
typedef std::wstring string_local;
typedef wchar_t char_local;
typedef std::wistringstream string_stream_local;
typedef struct _stat64i32 stat_local;
void MemcpyLocal(char_local* dst, const char_local* src, size_t size);
void MemcatLocal(char_local* dst, const char_local* src, size_t size);
bool StatLocal(const string_local& path, stat_local* stat);
char_local* ReadDiskFileDataLocal(const string_local& path, size_t& file_size);

//wstringstream待处理
//to_wstring待处理

//命令选项开关
struct OptionSwitch
{
	bool _s = false;
	bool _ad = false;
	bool _y = false;
	bool _d = false;
};


//返回类型
enum class ReturnType
{
	Success = 1000, 
	UnExpectedException,//未预期的错误
	DirNameIsInvalid,//虚拟磁盘目录名称无效
	AccessDenied,//虚拟磁盘路径拒绝访问
	MemoryDirIsNotEmpty,//虚拟磁盘目录不是空的
	DiskPathIsNotFound,//真实磁盘路径不存在
	DiskPathIsNotDirectoyOrFile,//真实磁盘路径既不是目录也不是文件

	MemoryPathIsNotFound,//虚拟磁盘路径不存在
	MemoryPathIsNotDirectoryOrFile,//虚拟磁盘路径既不是目录也不是文件

	MemoryPathNameIsIllegal,//不合法的路径名称
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

	//虚拟磁盘目录名称无效
	extern const string_local gsDirNameInvalid;

	//虚拟磁盘目录不是空的
	extern const string_local gsMemoryDirIsNotEmpty;

	//真实磁盘路径既不是目录也不是文件
	extern const string_local gsDiskPathIsNotDirectoyOrFile;
}

//字符集
namespace CharSet
{
	// ' '
	extern const char_local char_space;

	// "
	extern const char_local char_doublequote;

	// /
	extern const char_local char_slash;

	// '\'
	extern const char_local char_backslash;

	// :
	extern const char_local char_colon;

	// *
	extern const char_local char_asterisk;

	// ?
	extern const char_local char_question;

	// |
	extern const char_local char_or;

	// <
	extern const char_local char_lessthan;

	// >
	extern const char_local char_morethan;

	// @
	extern const char_local char_at;

	// .
	extern const char_local char_dot;

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
