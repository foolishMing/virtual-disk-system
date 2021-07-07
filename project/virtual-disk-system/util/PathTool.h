#pragma once
#ifndef __PATHTOOL_H__
#define __PATHTOOL_H__

#include "define.h"
#include <set>

class PathTools
{
public:
	static bool IsDiskPathExist(const string_local& path_str); //�жϴ���·���Ƿ����

	//�ж�tokens�Ƿ��﷨��ȷ
	static bool IsTokensFormatLegal(const std::vector<string_local>& tokens);

	//��·���ַ������Ϊtokens
	static bool SplitPathToTokens(const string_local& path_str, std::vector<string_local>& tokens);//path_str -> tokens

private:
	//(token��)�Ƿ��ַ���
    static std::set<char_local> g_illegal_path_charset;

	static void SplitPathToTokensInternal(const string_local& pure_path_str, std::vector<string_local>& tokens); //pure_path_str -> tokens
};



#endif // __PATHTOOL_H__
