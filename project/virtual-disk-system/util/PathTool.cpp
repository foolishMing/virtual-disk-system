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


//-��·���з�Ϊtokens
//1��ȥ����β�����Ŀո�
//2��ȥ����β��Ե�����
//3��token�еĿո�����ͨ�ַ�ɨ�账��
//4��token�п��Գ��������ַ�
bool PathTools::SplitPathToTokens(const string_local& path_str, std::vector<string_local>& tokens)
{
	auto path_str_trim = StringTools::Trimed(path_str);
	StringTools::StringDerefDoubleQuote(path_str_trim);
	auto str_len = path_str_trim.length();
	//�ݴ���
	if (0 == str_len)
	{
		Log::LogWarn(L"·��Ϊ��");
		return false;
	}
	//��ֳ�tokens
	if (CharSet::char_doublequote == path_str_trim[0])//���ַ�������
	{
		if (str_len < 2)
		{
			return false;
		}
		if (CharSet::char_doublequote != path_str_trim[str_len - 1])//β�ַ���������
		{
			return false;
		}
		auto deref_path_str = path_str_trim.substr(1, str_len - 2);//������
		SplitPathToTokensInternal(deref_path_str, tokens);
	}
	else//���ַ���������
	{
		SplitPathToTokensInternal(path_str_trim, tokens);
	}
	return true;
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


//-constrains
//1������/��\\��·�����зָ�
//2�����������ķָ��������"////"���Ӵ��ȼ���"/"
//3������token�д��ڿո񣬵�����ͨ��trim����ȥ����β�ո񣬲���trim��Ҫ�����п�
void PathTools::SplitPathToTokensInternal(const string_local& pure_path_str, std::vector<string_local>& tokens)
{
	string_local buffer = {};
	//ɨ��·��
	for(auto ch : pure_path_str)
	{
		if (CharSet::char_slash == ch || CharSet::char_backslash == ch)//�����ָ���
		{
			if (0 == buffer.length())//���������ķָ���
			{
				continue;
			}
			else//����������ȡ��һ��token 
			{
				buffer = StringTools::Trimed(buffer);//�Ƴ���β�ո�
				if (0 != buffer.length())//��token�����п�
				{
					tokens.push_back(buffer);
				}
				buffer = {};
			}
		}
		else//д������
		{
			buffer += ch;
		}
	}
	//��ջ�����
	if (0 != buffer.length())
	{
		tokens.push_back(buffer);
	}
}
