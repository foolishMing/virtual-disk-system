#pragma once
#ifndef __COMMANDARGS_H__
#define __COMMANDARGS_H__

#include "../util/Common.h"



class CommandArg
{
public :
	explicit CommandArg();
	virtual ~CommandArg();
	//һ���Ϸ���ָ��ṹӦΪ
	//command_type + args + paths
	//����CommandTypeһ����������ײ�
	//��args��paths����������˳������
	//paths���Խ�������Ϸ��ַ�
	void Analyse(const string_local& in);
	//�����ã���ʽ����ӡ�����б�
	void Print();

public:
	string_local cmd_token = L"";
	std::vector<string_local> options = {};	//ѡ���б�
	std::vector<string_local> paths = {};	//·���б�
	std::map<string_local, std::vector<string_local>>tokens_map;//path - tokens ӳ���
};

#endif // !__COMMANDARGS_H__
