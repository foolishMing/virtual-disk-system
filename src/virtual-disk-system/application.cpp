#include "application.h"
Application::Application()
{
	//std::cout << "call application ctor \n";
}

Application::~Application()
{
	//std::cout << "call application dtor \n";
}

void Application::printCurrentPath()
{
	std::cout << "C:>";
}

Application::RunStatus Application::exec(const std::string& strCmd)
{
	//退出程序
	if ("quit" == strCmd)
	{
		return RunStatus::exit;
	}
	//清屏
	if ("cls" == strCmd)
	{
		system("cls");
		return RunStatus::normal;
	}
	//路径类（测试）
	std::filesystem::path p(strCmd);
	if (exists(p)) {
		std::cout << strCmd << " : 是合法的路径" << std::endl;
		return RunStatus::normal;
	}
}
