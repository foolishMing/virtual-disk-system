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
	static enum RunStatus {										//程序运行状态
		normal = 0,
		exit,
	};
	explicit Application();
	virtual ~Application();
	virtual void create();
	virtual void destroy();
	void printCurrentPath();
	void readln(string_local& input);								//读取用户输入
	Application::RunStatus exec(const string_local& strCmd);		//返回程序运行状态
private:
	bool m_isCreate = false;
	NodeTree* m_node_tree = nullptr;	//文件树
	NodeTreeProxy* m_node_tree_proxy = nullptr;	//文件树代理
	CommandFactory* m_command_factory = nullptr; //指令工厂

	bool isPathExist(const string_local& str);
	std::vector<string_local> stringSplit(const string_local& in, const string_local& delimit = L" ");//字符串分割，默认分隔符为空格
};

#endif // !__APPLICATION_H__

