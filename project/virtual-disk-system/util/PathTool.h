#pragma once
#ifndef __PATHTOOL_H__
#define __PATHTOOL_H__

#include "define.h"
#include <set>
#include <windows.h>

class PathTools
{
public:

	//�жϴ���·���Ƿ����
	static bool IsDiskPathExist(const string_local& path_str); 

	//����·�������ļ���·����֧��ͨ���
	static bool SearchDiskFilesByWildcard(const char_local* wildcardPath, std::vector<string_local>& result_path_vec);

	//����·����ȡ�ļ���
	static string_local GetFileName(const string_local& path);

	//�ж�tokens�Ƿ��﷨��ȷ
	static bool IsTokensFormatLegal(const std::vector<string_local>& tokens);


private:
	//(token��)�Ƿ��ַ���
    static std::set<char_local> g_illegal_path_charset;

	//�����ļ�����ȡ����·��
	static bool GetFullDiskPath(const wchar_t* wildcardPath, const LPCWSTR file_name, string_local& full_path);

};



#endif // __PATHTOOL_H__
