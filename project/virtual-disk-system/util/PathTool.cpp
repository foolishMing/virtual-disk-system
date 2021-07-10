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
		//CharSet::char_colon, //:
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


//-将路径切分为tokens
//1、去除首尾连续的空格
//2、去除首尾配对的引号
//3、token中的空格当作普通字符扫描处理
//4、token中可以出现特殊字符
bool PathTools::SplitPathToTokens(const string_local& path_str, std::vector<string_local>& tokens)
{
	auto path_str_trim = StringTools::Trimed(path_str);
	StringTools::StringDerefDoubleQuote(path_str_trim);
	auto str_len = path_str_trim.length();
	//容错处理
	if (0 == str_len)
	{
		Log::LogWarn(L"路径为空");
		return false;
	}
	//拆分成tokens
	if (CharSet::char_doublequote == path_str_trim[0])//首字符是引号
	{
		if (str_len < 2)
		{
			return false;
		}
		if (CharSet::char_doublequote != path_str_trim[str_len - 1])//尾字符不是引号
		{
			return false;
		}
		auto deref_path_str = path_str_trim.substr(1, str_len - 2);//解引号
		SplitPathToTokensInternal(deref_path_str, tokens);
	}
	else//首字符不是引号
	{
		SplitPathToTokensInternal(path_str_trim, tokens);
	}
	return true;
}


//-constrains
//1、token是非空字符串
//2、token首尾不能有空格
//3、token中不能出现 /\ : * ? " < > | 等字符
bool PathTools::IsTokensFormatLegal(const std::vector<string_local>& tokens)
{
	for (auto item : tokens)
	{
		assert(!item.empty());//非空串
		assert(CharSet::char_space != item[0]);//无首空格
		assert(CharSet::char_space != item.back());//无尾空格
		for (auto ch : item)
		{
			if (PathTools::g_illegal_path_charset.count(ch))//存在非法字符
			{
				return false;
			}
		}
	}
	return true;
}


//-constrains
//1、基于/和\\对路径进行分割
//2、忽略连续的分割符，形如"////"的子串等价于"/"
//3、允许token中存在空格，但必须通过trim操作去掉首尾空格，并且trim后要进行判空
void PathTools::SplitPathToTokensInternal(const string_local& pure_path_str, std::vector<string_local>& tokens)
{
	string_local buffer = {};
	//扫描路径
	for(auto ch : pure_path_str)
	{
		if (CharSet::char_slash == ch || CharSet::char_backslash == ch)//遇到分隔符
		{
			if (0 == buffer.length())//忽略连续的分隔符
			{
				continue;
			}
			else//读缓冲区，取出一个token 
			{
				buffer = StringTools::Trimed(buffer);//移除首尾空格
				if (0 != buffer.length())//对token进行判空
				{
					tokens.push_back(buffer);
				}
				buffer = {};
			}
		}
		else//写缓冲区
		{
			buffer += ch;
		}
	}
	//清空缓冲区
	if (0 != buffer.length())
	{
		tokens.push_back(buffer);
	}
}
