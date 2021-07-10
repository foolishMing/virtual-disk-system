#include "Path.h"
#include "StringTool.h"
#include "PathTool.h"

Path::Path(const string_local& path)
{
	m_path = path;
}

Path::~Path()
{

}


//1、去除首尾连续的空格
//2、去除配对的引号
void Path::Split()
{
	//去除首尾连续空格
	m_path = StringTools::Trimed(m_path);
	assert(!m_path.empty());
	//去除配对的引号
	{
		string_local tmp_str = {};
		bool is_quote_open = StringTools::FilterMatchedDoubleQuotes(m_path, tmp_str);
		if (is_quote_open)
		{
			return;	//error: 路径名语法不正确，存在不配对的双引号
		}
		m_path = tmp_str;
	}
	if (m_path.empty())
	{
		return;	//error: 路径名语法不正确，去除双引号后路径为空
	}
	//路径名语法正确
	m_is_valid = SplitToTokensInternal();
}


//1、分隔符为\或/
//2、路径中的空格当作普通字符处理
bool Path::SplitToTokensInternal()
{
	m_tokens.clear();
	string_local buffer = {};
	//扫描路径
	for (auto ch : m_path)
	{
		//遇到分隔符
		if (CharSet::char_slash == ch || CharSet::char_backslash == ch)
		{
			//忽略连续的分隔符
			if (buffer.empty())
			{
				continue;
			}
			//移除首尾空格
			buffer = StringTools::Trimed(buffer);
			if (buffer.empty())
			{
				return false;//error : 路径名语法不正确，token是空串
			}			
			//读缓冲区，取出一个token 
			m_tokens.push_back(buffer);
			buffer = {};
			continue;
		}
		//写缓冲区
		buffer += ch;		
	}
	//清空缓冲区
	if (!buffer.empty())
	{
		buffer = StringTools::Trimed(buffer);
		if (buffer.empty())
		{
			return false;//error : 路径名语法不正确，token是空串
		}
		m_tokens.push_back(buffer);
	}
	return true;
}


const bool Path::IsWild() const 
{
	auto& str = m_tokens.back();
	assert(!str.empty());
	if (str.find(CharSet::char_question) != str.npos || str.find(CharSet::char_asterisk))
	{
		return true;
	}
	return false;
}
