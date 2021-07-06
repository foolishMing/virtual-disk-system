#pragma once
#ifndef __PATHTOOL_H__
#define __PATHTOOL_H__

#include "define.h"
#include <set>

class PathTools
{
public:
	static bool IsDiskPathExist(const string_local& path_str); //�жϴ���·���Ƿ����
	static bool IsLegalDiskPathFormat(const string_local& path_str); //�ж�һ���ַ����Ƿ�Ϊ�Ϸ�
	static bool IsPathFormatLegal(const string_local& path_str); //�ж�һ���ַ����Ƿ�Ϊ�Ϸ�·��
	static void SplitPathToTokens(const string_local& in, std::vector<string_local> tokens);
	static bool IsRelativePath(const string_local& path_str);//�ж�һ��·���Ƿ�Ϊ���·��	
private:
    static std::set<char_local> illegal_path_charset;
};



#endif // __PATHTOOL_H__
