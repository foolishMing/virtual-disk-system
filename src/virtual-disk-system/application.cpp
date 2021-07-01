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
	//输入输出本地化
	Console::Init();
	//创建命令工厂
	m_command_factory = new CommandFactory();
	m_command_factory->create();
	//创建文件树
	m_node_tree = new NodeTree();
	m_node_tree->create();
	//创建文件树代理
	m_node_tree_proxy = new NodeTreeProxy();
	m_node_tree_proxy->create();
	m_isCreate = true;
}

void Application::destroy()
{
	assert(m_isCreate == true);
	if (nullptr != m_command_factory)
	{
		m_command_factory->destroy();
		m_command_factory = nullptr;
	}
	if (nullptr != m_node_tree)
	{
		m_node_tree->destroy();
		m_node_tree == nullptr;
	}
	if (nullptr != m_node_tree_proxy)
	{
		m_node_tree->destroy();
		m_node_tree_proxy = nullptr;
	}
}

void Application::readln(string_local& input)
{
	Console::Read::readln(input);
}

void Application::printCurrentPath()
{
	assert(m_isCreate == true);
	Console::Write::print(L"C:>");
}

Application::RunStatus Application::exec(const string_local& strCmd)
{
	assert(m_isCreate == true);
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


//error:这个方法接收宽字符之后path创建失败
bool Application::isPathExist(const string_local& str)
{
	std::filesystem::path p(str);
	if (exists(p)) {
		return true;
	}
	return false;
}

std::vector<string_local> Application::stringSplit(const string_local& in, const string_local& delimit)
{
	std::vector<string_local> vec;
	string_local tmp = in;
	size_t nPos = in.find(delimit.c_str());
	while (string_local::npos != nPos)
	{
		string_local item = tmp.substr(0, nPos);
		vec.push_back(item);
		tmp = tmp.substr(nPos + delimit.length());
		nPos = tmp.find(delimit.c_str());
	}
	return vec;
}
