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
//1��������"@"��ͷ
//2��"@"����Ӵ�����Ϊ�Ϸ���·����ʽ
bool PathTools::IsLegalDiskPathFormat(const string_local& path_str)
{
	if (path_str.length() <= 1)
	{
		return false;
	}
	//�жϸ�·���Ƿ�Ϊ����·��
	if (L'@' != path_str[0])
	{
		return false;
	}
	//�жϸ�·���Ƿ�Ϸ�
	auto suffix_path_str = path_str.substr(1, path_str.size() - 1);
	if (IsPathFormatLegal(suffix_path_str))
	{
		return true;
	}
	return false;
}


//-constrains:
//1��·���в��ܳ��� /\ : * ? " < > | �ַ�
//2����������ţ����ж��Ƿ���β�ɶ�
//3��·��������ո���ڣ�������"    "��ֻ�ɿո���ɵ�token�ǲ��Ϸ���
bool PathTools::IsPathFormatLegal(const string_local& path_str)
{
	auto str_len = path_str.length();
	//�ݴ���
	if (0 == str_len)
	{
		Log::LogWarn(L"·��Ϊ��");
		return false;
	}
	//��ֳ�tokens
	std::vector<string_local> tokens = {};
	if (CharSet::char_doublequote == path_str[0])//���ַ�������
	{		
		if (str_len < 2)
		{
			return false;
		}
		if (CharSet::char_doublequote != path_str[str_len - 1])//β�ַ���������
		{
			return false;
		}
		SplitPathToTokens(path_str.substr(1, str_len - 2), tokens);
	}	
	else//���ַ���������
	{
		SplitPathToTokens(path_str, tokens);
	}	
	//�ж�tokens�Ƿ�Ϸ�
	for (auto item : tokens)
	{
		assert(0 != item.length());
		if (0 == StringTools::StringTrimed(item).length())//ֻ�пո��ַ���token�ǲ��Ϸ���
		{
			return false;
		}
		for (auto ch : item)
		{
			if (PathTools::illegal_path_charset.count(ch))//���ڷǷ��ַ�
			{
				return false;
			}
		}
	}
	return true;
}


//-constrains:
//1������/��\\��·�����зָ�
//2�����������ķָ��������"////"���Ӵ��ȼ���"/"
void PathTools::SplitPathToTokens(const string_local& in, std::vector<string_local> tokens)
{
	string_local buffer = {};
	//·���ָ�
	for(auto item : in)
	{
		if (CharSet::char_slash == item || CharSet::char_backslash == item)
		{
			if (0 == buffer.length())//������"////"�������ָ������д���
			{
				continue;
			}
			else//�����һ��������token������ջ����� 
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
	//��ջ�����
	if (0 != buffer.length())
	{
		tokens.push_back(buffer);
	}
}
