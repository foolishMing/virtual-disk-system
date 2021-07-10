#pragma once
#ifndef __COMMANDARGS_H__
#define __COMMANDARGS_H__

#include "../util/Common.h"



class CommandArg
{
public :
	explicit CommandArg();
	virtual ~CommandArg();
	//һ���Ϸ���ָ��ṹӦΪ{cmd_type + options + paths}
	//����cmd_typeһ����������ײ�
	//��options��paths����������˳������
	//option�ĵ�һ���ַ�һ����/
	//path�п�����˫���š��ո�ͨ�����*��?��
	void Analyse(const string_local& in);
	//�����ã���ʽ����ӡ�����б�
	void Print();

public:
	string_local cmd_token = L"";
	std::vector<string_local> options = {};	//ѡ���б�
	std::vector<Path> paths = {};	//·���б�
};

#endif // !__COMMANDARGS_H__
