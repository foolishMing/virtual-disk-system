#include "PathTool.h"
#include "Log.h"
#include "StringTool.h"

std::set<char_local> initial_illegal_charset()
{
	std::set<char_local> ret = 
	{ 
		CharSet::char_doublequote,//'"'
		CharSet::char_slash,//'/'
		CharSet::char_backslash,//'\\'
		CharSet::char_asterisk,//*
		CharSet::char_colon, //:
		CharSet::char_question, //?
		CharSet::char_lessthan,//<
		CharSet::char_morethan,//>
		CharSet::char_or,//|
	};
	return ret;
}
std::set<char_local> PathTools::g_illegal_path_charset(initial_illegal_charset());

bool PathTools::IsDiskPathExist(const string_local& path_str)
{
	std::filesystem::path p(path_str);
	if (exists(p)) {
		return true;
	}
	return false;
}




//-constrains
//1��token�Ƿǿ��ַ���
//2��token��β�����пո�
//3��token�в��ܳ��� /\ : * ? " < > | ���ַ�
bool PathTools::IsTokensFormatLegal(const std::vector<string_local>& tokens)
{
	for (auto item : tokens)
	{
		assert(!item.empty());//�ǿմ�
		assert(CharSet::char_space != item[0]);//���׿ո�
		assert(CharSet::char_space != item.back());//��β�ո�
		for (auto ch : item)
		{
			if (PathTools::g_illegal_path_charset.count(ch))//���ڷǷ��ַ�
			{
				return false;
			}
		}
	}
	return true;
}


//��ȡ�ļ�������·��
bool PathTools::GetFullDiskPath(const wchar_t* wildcardPath, const LPCWSTR file_name, string_local& full_path)
{
	string_local path(wildcardPath);
	int pos_slash = path.find_last_of(CharSet::char_slash);
	int pos_back_slash = path.find_last_of(CharSet::char_backslash);
	int pos_min = min(pos_slash, pos_back_slash);
	int pos_max = max(pos_slash, pos_back_slash);
	//����pos_max���ǰ׺��
	if (pos_max < path.npos)
	{
		string_local path_prefix = path.substr(0, static_cast<std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>::size_type>(pos_max) + 1);
		full_path = path_prefix + file_name;
	}
	//����pos_min���ǰ׺��
	else if (pos_min < path.npos)
	{
		string_local path_prefix = path.substr(0, static_cast<std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>::size_type>(pos_min) + 1);
		full_path = path_prefix + file_name;
	}
	//���ָ�
	else
	{
		full_path = file_name;
	}
	return true;
}

//����ͨ��������ļ�
bool PathTools::SearchDiskFilesByWildcard(const char_local* wildcardPath, std::vector<string_local>& result_path_vec)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA pNextInfo;
	//���ҵ�һ���ļ�
	hFile = FindFirstFile(wildcardPath, &pNextInfo);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}
	if (pNextInfo.cFileName[0] != CharSet::char_dot)
	{
		string_local full_path = {};
		auto ret = GetFullDiskPath(wildcardPath, pNextInfo.cFileName, full_path);
		if (ret == true)
		{
			if (pNextInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{

			}
			else 
			{
				result_path_vec.emplace_back(full_path);
			}
		}
	}
	//������һ���ļ�
	while (FindNextFile(hFile, &pNextInfo))
	{
		if (pNextInfo.cFileName[0] == CharSet::char_dot)
		{
			continue;
		}
		string_local full_path = {};
		auto ret = GetFullDiskPath(wildcardPath, pNextInfo.cFileName, full_path);
		if (ret == true)
		{
			if (pNextInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{

			}
			else
			{
				result_path_vec.emplace_back(full_path);
			}
		}
	}
	return true;
}

string_local PathTools::GetFileName(const string_local& path)
{
	string_local ret = {};
	int pos_slash = path.find_last_of(CharSet::char_slash);
	int pos_back_slash = path.find_last_of(CharSet::char_backslash);
	int pos_min = min(pos_slash, pos_back_slash);
	int pos_max = max(pos_slash, pos_back_slash);
	int split_pos = 0;
	//����pos_max��ú�׺��
	if (pos_max < path.npos)
	{
		split_pos = pos_max + 1;
	}
	//����pos_min��ú�׺��
	else if (pos_min < path.npos)
	{
		split_pos = pos_min + 1;
	}
	ret = path.substr(split_pos, path.length() - split_pos);
	return ret;
}
