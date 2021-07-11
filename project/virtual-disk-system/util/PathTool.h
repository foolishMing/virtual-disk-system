#pragma once
#ifndef __PATHTOOL_H__
#define __PATHTOOL_H__

#include "define.h"
#include <set>
#include <windows.h>

class PathTools
{
public:

	//判断磁盘路径是否存在
	static bool IsDiskPathExist(const string_local& path_str); 

	//基于路径查找文件，路径中支持通配符
	static bool SearchDiskFilesByWildcard(const char_local* wildcardPath, std::vector<string_local>& result_path_vec);

	//基于路径获取文件名
	static string_local GetFileName(const string_local& path);

	//判断tokens是否语法正确
	static bool IsTokensFormatLegal(const std::vector<string_local>& tokens);


private:
	//(token的)非法字符集
    static std::set<char_local> g_illegal_path_charset;

	//根据文件名获取完整路径
	static bool GetFullDiskPath(const wchar_t* wildcardPath, const LPCWSTR file_name, string_local& full_path);

};



#endif // __PATHTOOL_H__
