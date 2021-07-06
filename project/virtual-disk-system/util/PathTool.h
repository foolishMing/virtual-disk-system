#pragma once
#ifndef __PATHTOOL_H__
#define __PATHTOOL_H__

#include "define.h"
#include <set>

class PathTools
{
public:
	static bool IsDiskPathExist(const string_local& path_str); //判断磁盘路径是否存在
	static bool IsLegalDiskPathFormat(const string_local& path_str); //判断一个字符串是否为合法
	static bool IsPathFormatLegal(const string_local& path_str); //判断一个字符串是否为合法路径
	static void SplitPathToTokens(const string_local& in, std::vector<string_local> tokens);
	static bool IsRelativePath(const string_local& path_str);//判断一个路径是否为相对路径	
private:
    static std::set<char_local> illegal_path_charset;
};



#endif // __PATHTOOL_H__
