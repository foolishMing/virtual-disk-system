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
#include "./node/NodeTreeProxy.h"

class Application : Object {
public:
	static enum RunStatus {										//��������״̬
		normal = 0,
		exit,
	};
	explicit Application();
	virtual ~Application();
	virtual void create();
	virtual void destroy();
	void printCurrentPath();
	void readln(string_local& input);								//��ȡ�û�����
	Application::RunStatus exec(const string_local& strCmd);		//���س�������״̬
private:
	bool m_isCreate = false;
	NodeTree* m_node_tree = nullptr;	//�ļ���
	NodeTreeProxy* m_node_tree_proxy = nullptr;	//�ļ�������
	CommandFactory* m_command_factory = nullptr; //ָ���

	bool isPathExist(const string_local& str);
	std::vector<string_local> stringSplit(const string_local& in, const string_local& delimit = L" ");//�ַ����ָĬ�Ϸָ���Ϊ�ո�
};

#endif // !__APPLICATION_H__

