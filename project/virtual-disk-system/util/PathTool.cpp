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
std::set<char_local> PathTools::illegal_path_charset(initial_illegal_charset());

bool PathTools::IsDiskPathExist(const string_local& path_str)
{
	std::filesystem::path p(path_str);
	if (exists(p)) {
		return true;
	}
	return false;
}

//-constrains:
//1、必须以"@"开头
//2、"@"后的子串必须为合法的路径格式
bool PathTools::IsLegalDiskPathFormat(const string_local& path_str)
{
	if (path_str.length() <= 1)
	{
		return false;
	}
	//判断该路径是否为磁盘路径
	if (L'@' != path_str[0])
	{
		return false;
	}
	//判断该路径是否合法
	auto suffix_path_str = path_str.substr(1, path_str.size() - 1);
	if (IsPathFormatLegal(suffix_path_str))
	{
		return true;
	}
	return false;
}


//-constrains:
//1、路径中不能出现 /\ : * ? " < > | 字符
//2、如果有引号，则判断是否首尾成对
//3、路径中允许空格存在，但形如"    "的只由空格组成的token是不合法的
bool PathTools::IsPathFormatLegal(const string_local& path_str)
{
	auto str_len = path_str.length();
	//容错处理
	if (0 == str_len)
	{
		Log::LogWarn(L"路径为空");
		return false;
	}
	//拆分成tokens
	std::vector<string_local> tokens = {};
	if (CharSet::char_doublequote == path_str[0])//首字符是引号
	{		
		if (str_len < 2)
		{
			return false;
		}
		if (CharSet::char_doublequote != path_str[str_len - 1])//尾字符不是引号
		{
			return false;
		}
		SplitPathToTokens(path_str.substr(1, str_len - 2), tokens);
	}	
	else//首字符不是引号
	{
		SplitPathToTokens(path_str, tokens);
	}	
	//判断tokens是否合法
	for (auto item : tokens)
	{
		assert(0 != item.length());
		if (0 == StringTools::StringTrimed(item).length())//只有空格字符的token是不合法的
		{
			return false;
		}
		for (auto ch : item)
		{
			if (PathTools::illegal_path_charset.count(ch))//存在非法字符
			{
				return false;
			}
		}
	}
	return true;
}


//-constrains:
//1、基于/和\\对路径进行分割
//2、忽略连续的分割符，形如"////"的子串等价于"/"
void PathTools::SplitPathToTokens(const string_local& in, std::vector<string_local> tokens)
{
	string_local buffer = {};
	//路径分割
	for(auto item : in)
	{
		if (CharSet::char_slash == item || CharSet::char_backslash == item)
		{
			if (0 == buffer.length())//对形如"////"的连续分隔符进行处理
			{
				continue;
			}
			else//获得了一个完整的token，并清空缓冲区 
			{
				tokens.push_back(buffer);
				buffer = {};
			}
		}
		else
		{
			buffer += item;
		}
	}
	//清空缓冲区
	if (0 != buffer.length())
	{
		tokens.push_back(buffer);
	}
}
