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
	//�˳�����
	if ("quit" == strCmd)
	{
		return RunStatus::exit;
	}
	//����
	if ("cls" == strCmd)
	{
		system("cls");
		return RunStatus::normal;
	}
	//·���ࣨ���ԣ�
	std::filesystem::path p(strCmd);
	if (exists(p)) {
		std::cout << strCmd << " : �ǺϷ���·��" << std::endl;
		return RunStatus::normal;
	}
}
