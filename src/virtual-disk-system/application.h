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
		normal,
		exit,
	};
	explicit Application();
	virtual ~Application();
	void printCurrentPath();
	Application::RunStatus exec(const std::string& strCmd);		//返回程序运行状态
private:

};
