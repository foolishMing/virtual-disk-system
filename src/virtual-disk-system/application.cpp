#include "application.h"
#include "./util/Console.hpp"

Application::Application()
{
}

Application::~Application()
{
}


void Application::create()
{
	m_isCreate = true;
	Console::Init();
}

void Application::readln(std::wstring& input)
{
	Console::Read::readln(input);
}

void Application::printCurrentPath()
{
	Console::Write::print(L"C:>");
}

Application::RunStatus Application::exec(const std::wstring& strCmd)
{
	if (!m_isCreate) 
	{
		Console::Write::println(L"应用程序未初始化");
		return RunStatus::exit;
	}
	//退出程序
	if (L"quit" == strCmd || L"exit" == strCmd)
	{
		return RunStatus::exit;
	}
	//清屏
	if (L"cls" == strCmd)
	{
		system("cls");
		return RunStatus::normal;
	}
	//路径类（测试）
	std::filesystem::path p(strCmd);
	if (exists(p)) {
		Console::Write::print(strCmd);
		Console::Write::println(L" : 是合法的真实路径");
		return RunStatus::normal;
	}
	return RunStatus::normal;
}
