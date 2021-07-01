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
	//����������ػ�
	Console::Init();
	//���������
	m_command_factory = new CommandFactory();
	m_command_factory->create();
	//�����ļ���
	m_node_tree = new NodeTree();
	m_node_tree->create();
	//�����ļ�������
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
		Console::Write::println(L"Ӧ�ó���δ��ʼ��");
		return RunStatus::exit;
	}
	//�˳�����
	if (L"quit" == strCmd || L"exit" == strCmd)
	{
		return RunStatus::exit;
	}
	//����
	if (L"cls" == strCmd)
	{
		system("cls");
	}
	//·���ࣨ���ԣ�
	else if (isPathExist(strCmd))
	{
		Console::Write::print(strCmd);
		Console::Write::println(L" �ǺϷ�����ʵ·��");
	}
	return RunStatus::normal;
}


//error:����������տ��ַ�֮��path����ʧ��
bool Application::isPathExist(const string_local& str)
{
	std::filesystem::path p(str);
	if (exists(p)) {
		return true;
	}
	return false;
}
