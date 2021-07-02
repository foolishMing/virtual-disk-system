#pragma once
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <atlstr.h>
#include <cstring>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include "./command/BaseCommand.h"
#include "./command/CommandFactory.h"
#include "./node/NodeTree.h"
#include "./node/NodeTreeManager.h"

class Application : Object {
public:
    enum RunStatus {										//��������״̬
		normal = 0,
		exit,
	};
	explicit Application();
	virtual ~Application();
	virtual void Create();
	virtual void Destroy();
	void Run();
	void PrintCurrentPath();
	void ReadLine(string_local& input);								//��ȡ�û�����
	Application::RunStatus ExecCommand(const string_local& strCmd);		//���س�������״̬
private:
	bool m_isCreate = false;

	NodeTreeManager* m_node_tree_manager = nullptr;	//�ļ���������
	CommandFactory* m_command_factory = nullptr; //ָ���

	bool IsPathExist(const string_local& str);
	std::vector<string_local> StringSplit(const string_local& in, const string_local& delimit = L" ");//�ַ����ָĬ�Ϸָ���Ϊ�ո�
	std::vector<string_local> StringSplits(const string_local& in, const std::vector<string_local>& delimits); //�ַ����ָ����ָ����б�
};

#endif // !__APPLICATION_H__

