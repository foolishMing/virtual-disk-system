#pragma once
#ifndef __PATHTOOL_H__
#define __PATHTOOL_H__

#include "define.h"
#include <set>

class PathTools
{
public:
	static bool IsDiskPathExist(const string_local& path_str); //判断磁盘路径是否存在

	//判断tokens是否语法正确
	static bool IsTokensFormatLegal(const std::vector<string_local>& tokens);

	//将路径字符串拆分为tokens
	static bool SplitPathToTokens(const string_local& path_str, std::vector<string_local>& tokens);//path_str -> tokens

private:
	//(token的)非法字符集
    static std::set<char_local> g_illegal_path_charset;

	static void SplitPathToTokensInternal(const string_local& pure_path_str, std::vector<string_local>& tokens); //pure_path_str -> tokens
};



#endif // __PATHTOOL_H__
