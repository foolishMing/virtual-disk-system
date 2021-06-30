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
	}
	//路径类（测试）
	else if (isPathExist(strCmd))
	{
		Console::Write::print(strCmd);
		Console::Write::println(L" 是合法的真实路径");
	}
	return RunStatus::normal;
}

bool Application::isPathExist(const std::wstring& str)
{
	std::filesystem::path p(str);
	if (exists(p)) {
		return true;
	}
	return false;
}
