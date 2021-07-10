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
	while (str >> buf) 
	{
		out.push_back(buf);
	}
}

/*
-constraints:
1���Կո�Ϊ�ָ���
2�������еĿո�����ͨ�ַ�����
*/
bool StringTools::StringSplitBySpaceAndQuotes(const string_local& in, std::vector<string_local>& out)
{
	if (in.empty())
	{
		Log::LogWarn(L"���Ϸ��Ĳ�������ͼ�Կմ�����Split()����");
		return false;
	}
	bool is_quote_open = false;	
	string_local buffer = {};
	for (auto item : in)
	{
		//�����ո�
		if (item == CharSet::char_space) 
		{
			//�������У��ո�����ͨ�ַ�����
			if (is_quote_open)
			{	
				buffer += item;
				continue;
			}
			//���������Ŀո�
			if (buffer.empty())
			{
				continue;
			}
			//�ӻ���������һ��token
			out.push_back(buffer);
			buffer = {};
		}
		//��������
		else if (item == CharSet::char_doublequote)//�������ţ��ı�����״̬
		{
			is_quote_open = !is_quote_open;
			buffer += item ;
		}
		else
		{
			buffer += item;
		}
	}
	//��ջ�����
	if (!buffer.empty())
	{
		out.push_back(buffer);
	}
	//����δƥ��ĵ����ţ����Ϸ�������
	if (is_quote_open) 
	{ 
		return false;
	}
	return true;
}


string_local StringTools::Trimed(const string_local& in)
{
	string_local out = in;
	if (out.empty())
	{
		Log::LogWarn(L"���Ϸ��Ĳ�������ͼ�Կմ�����Trimed()����");
		return out;
	}
	out.erase(0, out.find_first_not_of(L" "));
	out.erase(out.find_last_not_of(L" ") + 1);
	return out;
}

string_local StringTools::ToLowercase(const string_local& s)
{
	string_local ret = s;
	for (auto& ch : ret)
	{
		if (ch >= L'A' && ch <= L'Z')
		{
			ch = ch - L'A' + L'a';
		}
	}
	return ret;
}

bool StringTools::HasUppercase(const string_local& s)
{
	for (const auto ch : s)
	{
		if (ch >= L'A' && ch <= L'Z')
		{
			return true;
		}
	}
	return false;
}

//-constraints
//windows�µ������к��ļ����ǲ����ִ�Сд�ģ�
//�����ڽ����ַ����Ƚ�ʱ��Ӧ�����Դ�Сд��
//-to update
//�Ƚ����봮ת����Сд�����ٽ��бȽϣ������ζ���Ŀ������쿪����
//���Ƶ�����ã�Ӧ�����������Ż�
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

//-constraints
//���봮�ĳ��Ȳ���Ϊ0
string_local StringTools::GetStringSuffix(const string_local& in, size_t cnt)
{
	assert(!in.empty());
	if (in.empty())
	{
		Log::LogWarn(L"���Ϸ��Ĳ�������ͼ��ȡ�մ��ĺ�׺�Ӵ�");
		return L"";
	}
	int max_len = static_cast<int>(std::min(in.length(), cnt));
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

void StringTools::StringDerefDoubleQuote(string_local& s)
{
	if (s.length() < 2)
	{
		return;
	}
	if (s[0] == CharSet::char_doublequote && s.back() == CharSet::char_doublequote)
	{
		s = s.substr(1, s.length() - 2);
	}
}

//false:���е�˫���Ŷ�����Ե�
bool StringTools::FilterMatchedDoubleQuotes(const string_local& src, string_local& dst)
{
	dst = {};
	bool is_quote_open = false;
	for (int i = 0; i < src.length(); i++)
	{
		if (src[i] == CharSet::char_doublequote)
		{
			is_quote_open = !is_quote_open;
		}
		else
		{
			dst += src[i];
		}
	}
	return is_quote_open;
}

bool StringTools::HasWildcard(const std::vector<string_local>& tokens)
{
	for (const auto& item : tokens)
	{
		if (item.find(CharSet::char_asterisk) != item.npos || item.find(CharSet::char_question) != item.npos)
		{
			return true;
		}
	}
	return false;
}


string_local StringTools::TimeStampToDateTimeString(time_t ts)
{
	return L"0000/00/00  00:00";
}

