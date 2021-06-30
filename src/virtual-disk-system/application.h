#pragma once
#include <atlstr.h>
#include <cstring>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "./command/BaseCommand.h"


class Application {
public:
	static enum RunStatus{										//程序运行状态
		normal = 0,
		exit,
	};
	explicit Application();
	virtual ~Application();
	void create();
	void printCurrentPath();
	void readln(std::wstring& input);								//读取用户输入
	Application::RunStatus exec(const std::wstring& strCmd);		//返回程序运行状态
private:
	bool m_isCreate = false;
};
