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
		normal = 0,
		exit,
	};
	explicit Application();
	virtual ~Application();
	void create();
	void printCurrentPath();
	void readln(std::wstring& input);								//��ȡ�û�����
	Application::RunStatus exec(const std::wstring& strCmd);		//���س�������״̬
private:
	bool m_isCreate = false;
};
