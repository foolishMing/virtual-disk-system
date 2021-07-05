#include "StringTool.h"
#include "Log.h"

void StringTools::StringSplitBySpace(const string_local& in, std::vector<string_local>& out)
{
	if (in.empty())
	{
		Log::LogWarn(L"���Ϸ��Ĳ�������ͼ�Կմ�����Split()����");
		return;
	}
	string_stream_local str(in);
	string_local buf;
	while (str >> buf) {
		out.push_back(buf);
	}
}

bool StringTools::StringSplitBySpaceWithQuotes(const string_local& in, std::vector<string_local>& out)
{
	if (in.empty())
	{
		Log::LogWarn(L"���Ϸ��Ĳ�������ͼ�Կմ�����Split()����");
		return false;
	}
	bool is_quote = false;	
	auto char_space = L' ';
	auto char_quote = L'"';
	string_local buf = {};
	for (auto item : in)
	{
		if (item == char_space) {//�����ո�
			if (is_quote) //�������У�����ͨ�ַ�����
			{	
				buf += item;
				continue;
			}
			out.push_back(buf);
			buf = {};
		}
		else if (item == char_quote)//�������ţ��ı�����״̬
		{
			is_quote = !is_quote;
			buf += item ;
		}
		else
		{
			buf += item;
		}
	}
	//��ȡ���һ���ַ�������ջ�����
	if (!buf.empty())
	{
		out.push_back(buf);
		buf = {};
	}
	if (is_quote == true) { //����δƥ��ĵ����ţ����Ϸ�������
		return false;
	}
	return true;
}


string_local& StringTools::StringTrimed(string_local& s)
{
	if (s.empty())
	{
		Log::LogWarn(L"���Ϸ��Ĳ�������ͼ�Կմ�����Trimed()����");
		return s;
	}
	s.erase(0, s.find_first_not_of(L" "));
	s.erase(s.find_last_not_of(L" ") + 1);
	return s;
}

string_local StringTools::ToLowercase(const string_local& s)
{
	string_local ret = s;
	for (auto item : s)
	{
		if (item >= 'A' && item <= 'Z')
		{
			item = item - 'A' + 'a';
		}
	}
	return ret;
}

bool StringTools::HasUppercase(const string_local& s)
{
	for (auto item : s)
	{
		if (item >= 'A' && item <= 'Z')
		{
			return true;
		}
	}
	return false;
}


bool StringTools::IsEqual(const string_local& lhs, const string_local& rhs)
{
	auto lc_lhs = ToLowercase(lhs);
	auto lc_rhs = ToLowercase(rhs);
	if (lc_lhs == lc_rhs)
	{
		return true;
	}
	return false;
}

//Լ����in�ĳ��Ȳ���Ϊ0
string_local StringTools::GetStringSuffix(const string_local& in, size_t cnt)
{
	assert(!in.empty());
	if (in.empty())
	{
		Log::LogWarn(L"���Ϸ��Ĳ�������ͼ��ȡ�մ��ĺ�׺�Ӵ�");
		return L"";
	}
	int max_len = std::min(in.length(), cnt);
	return in.substr(in.size() - cnt, cnt);
}

//String�ϸ�ƥ�䣨���Դ�Сд��
bool StringTools::IsStringStrictEqualTo(const string_local& lhs, const string_local& rhs)
{
	auto lc_lhs = ToLowercase(lhs);
	auto lc_rhs = ToLowercase(rhs);
	return (lc_lhs == lc_rhs);
}

//Stringģ��ƥ��
bool StringTools::IsStringFuzzyEqualTo(const string_local& text, const string_local& pattern)
{
	auto lc_text = ToLowercase(text);
	auto lc_pattern = ToLowercase(pattern);
	if (IsFuzzyMatch(lc_text.c_str(), lc_pattern.c_str()))
	{
		return true;
	}
	return IsStringStrictEqualTo(text, pattern);
}

//CStringģ��ƥ�䣬֧��ͨ���"*"��"?"
bool StringTools::IsFuzzyMatch(const char_local* str, const char_local* pattern)
{
	if (*pattern == L'\0')
	{
		return true;
	}
	if (*pattern == L'*')
	{
		while (*str != L'\0')
		{
			if (IsFuzzyMatch((const char_local*)str++, (const char_local*)pattern + 1))
			{
				return true;
			}
		}
	}
	if (*str == L'\0')
	{
		return false;
	}
	if (*str == *pattern || *pattern == L'?')
	{
		return IsFuzzyMatch(str + 1, pattern + 1);
	}
	if (*str != *pattern)
	{
		return false;
	}
	return false;
}