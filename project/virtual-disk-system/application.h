#pragma once
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <atlstr.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include "./command/BaseCommand.h"
#include "./command/CommandFactory.h"
#include "./node/NodeTree.h"
#include "./node/NodeTreeManager.h"

class Application : Object {
public:
    enum class RunStatus { //程序运行状态
		normal = 0,
		quit,
	};
	explicit Application();
	virtual ~Application();
	virtual void Create();
	virtual void Destroy();
	void Run();
	void PrintCurrentPath();
	Application::RunStatus ExecCommand(const string_local& strCmd); //返回程序运行状态
private:
	bool m_isCreate = false;
	NodeTreeManager m_node_tree_manager; //文件树管理类
	CommandFactory* m_cmd_factory = nullptr; //指令工厂
};

#endif // !__APPLICATION_H__

