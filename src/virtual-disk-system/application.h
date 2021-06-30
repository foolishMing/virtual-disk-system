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
	static enum RunStatus{										//��������״̬
		normal,
		exit,
	};
	explicit Application();
	virtual ~Application();
	void printCurrentPath();
	Application::RunStatus exec(const std::string& strCmd);		//���س�������״̬
private:

};
