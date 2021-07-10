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


//1��ȥ����β�����Ŀո�
//2��ȥ����Ե�����
void Path::Split()
{
	//ȥ����β�����ո�
	m_path = StringTools::Trimed(m_path);
	assert(!m_path.empty());
	//ȥ����Ե�����
	{
		string_local tmp_str = {};
		bool is_quote_open = StringTools::FilterMatchedDoubleQuotes(m_path, tmp_str);
		if (is_quote_open)
		{
			return;	//error: ·�����﷨����ȷ�����ڲ���Ե�˫����
		}
		m_path = tmp_str;
	}
	if (m_path.empty())
	{
		return;	//error: ·�����﷨����ȷ��ȥ��˫���ź�·��Ϊ��
	}
	//·�����﷨��ȷ
	m_is_valid = SplitToTokensInternal();
}


//1���ָ���Ϊ\��/
//2��·���еĿո�����ͨ�ַ�����
bool Path::SplitToTokensInternal()
{
	m_tokens.clear();
	string_local buffer = {};
	//ɨ��·��
	for (auto ch : m_path)
	{
		//�����ָ���
		if (CharSet::char_slash == ch || CharSet::char_backslash == ch)
		{
			//���������ķָ���
			if (buffer.empty())
			{
				continue;
			}
			//�Ƴ���β�ո�
			buffer = StringTools::Trimed(buffer);
			if (buffer.empty())
			{
				return false;//error : ·�����﷨����ȷ��token�ǿմ�
			}			
			//����������ȡ��һ��token 
			m_tokens.push_back(buffer);
			buffer = {};
			continue;
		}
		//д������
		buffer += ch;		
	}
	//��ջ�����
	if (!buffer.empty())
	{
		buffer = StringTools::Trimed(buffer);
		if (buffer.empty())
		{
			return false;//error : ·�����﷨����ȷ��token�ǿմ�
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
