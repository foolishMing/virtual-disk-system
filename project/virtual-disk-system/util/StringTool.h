#pragma once 

#ifndef __STRINGTOOL_H__
#define __STRINGTOOL_H__
#include "Define.h"
#include "Log.h"

/*
unit test��
StringSplitBySpaceWithQuotes(); //�ַ����ָ�(���ڿո�)
IsStringFuzzyEqualTo();	//ģ��ƥ��
*/

class StringTools
{
public:
	//�ַ����ָ���ڿո�
	static void StringSplitBySpace(const string_local& in, std::vector<string_local>& out);
	//�ַ����ָ���ڿո񣩣������������Ű����Ŀո�
	static bool StringSplitBySpaceWithQuotes(const string_local& in, std::vector<string_local>& out);
	//ȥ����β�ո�
	static string_local StringTrimed(const string_local& s);
	//�ж��ַ����Ƿ����
	static bool IsEqual(const string_local& lhs, const string_local& rhs);
	//�ַ���תСд
	static string_local ToLowercase(const string_local& s);
	//�ж��ַ������Ƿ��д�д��ĸ
	static bool HasUppercase(const string_local& s);
	//��ȡ����Ϊsize�ĺ�׺�Ӵ�
	static string_local GetStringSuffix(const string_local& in, size_t cnt);

	//String�ϸ�ƥ�䣨���Դ�Сд��
	static bool IsStringStrictEqualTo(const string_local& lhs, const string_local& rhs);
	//Stringģ��ƥ��
	static bool IsStringFuzzyEqualTo(const string_local& text, const string_local& pattern);
	//CStringģ��ƥ�䣬֧��ͨ���"*"��"?"
	static bool IsFuzzyMatch(const char_local* str, const char_local* pattern);
};

#endif // !__STRINGTOOL_H__
