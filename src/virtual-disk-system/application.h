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
	static enum RunStatus {										//程序运行状态
		normal = 0,
		exit,
	};
	explicit Application();
	virtual ~Application();
	virtual void Create();
	virtual void Destroy();
	void PrintCurrentPath();
	void ReadLine(string_local& input);								//读取用户输入
	Application::RunStatus Exec(const string_local& strCmd);		//返回程序运行状态
private:
	bool m_isCreate = false;
	NodeTree* m_node_tree = nullptr;	//文件树

	NodeTreeManager* m_node_tree_manager = nullptr;	//文件树管理类
	CommandFactory* m_command_factory = nullptr; //指令工厂

	bool IsPathExist(const string_local& str);
	std::vector<string_local> StringSplit(const string_local& in, const string_local& delimit = L" ");//字符串分割，默认分隔符为空格
};

#endif // !__APPLICATION_H__

