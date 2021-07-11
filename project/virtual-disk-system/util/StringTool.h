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
	static bool StringSplitBySpaceAndQuotes(const string_local& in, std::vector<string_local>& out);
	//ȥ����β�ո�
	static string_local Trimed(const string_local& s);
	//ȥ����β����
	static void StringDerefDoubleQuote(string_local& s);

	//ȥ��ƥ���˫����
	static bool FilterMatchedDoubleQuotes(const string_local& src, string_local& dst);

	//�ж��ַ����Ƿ����
	static bool IsEqual(const string_local& lhs, const string_local& rhs);

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

	//�ж��Ƿ����ͨ���
	static bool HasWildcard(const std::vector<string_local>& tokens);

	//��ʱ���ת�ַ���
	static string_local TimeStampToDateTimeString(time_t ts);

	//����ת��ʽ�����ַ���
	static string_local FormatFromNumber(int num);

private:
	//�ַ���תСд
	static string_local ToLowercase(const string_local& s);
};

#endif // !__STRINGTOOL_H__
