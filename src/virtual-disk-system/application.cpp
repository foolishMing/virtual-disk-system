#include "application.h"
#include "./util/Banner.h"
#include "./util/Console.hpp"

Application::Application()
{
}

Application::~Application()
{

}

void Application::Create()
{
	//����������ػ�
	Console::Init();
	//�����ҳ���
	PrintBanner();
	//���������
	m_command_factory = new CommandFactory();
	m_command_factory->Create();
	//�����ļ���
	m_node_tree = new NodeTree();
	m_node_tree->Create();
	//�����ļ�������
	m_node_tree_proxy = new NodeTreeProxy();
	m_node_tree_proxy->Create();
	m_isCreate = true;
}

void Application::Destroy()
{
	assert(m_isCreate == true);
	if (nullptr != m_command_factory)
	{
		m_command_factory->Destroy();
		m_command_factory = nullptr;
	}
	if (nullptr != m_node_tree)
	{
		m_node_tree->Destroy();
		m_node_tree = nullptr;
	}
	if (nullptr != m_node_tree_proxy)
	{
		m_node_tree->Destroy();
		m_node_tree_proxy = nullptr;
	}
}

void Application::ReadLine(string_local& input)
{
	Console::Read::ReadLine(input);
}

void Application::PrintCurrentPath()
{
	assert(m_isCreate == true);
	Console::Write::Print(L"C:>");
}

Application::RunStatus Application::Exec(const string_local& strCmd)
{
	assert(m_isCreate == true);
	if (!m_isCreate) 
	{
		Console::Write::PrintLine(L"Ӧ�ó���δ��ʼ��");
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
		PrintBanner();
	}
	//·���ࣨ���ԣ�
	else if (IsPathExist(strCmd))
	{
		Console::Write::Print(strCmd);
		Console::Write::PrintLine(L" �ǺϷ�����ʵ·��");
	}
	return RunStatus::normal;
}


//error:����������տ��ַ�֮��path����ʧ��
bool Application::IsPathExist(const string_local& str)
{
	std::filesystem::path p(str);
	if (exists(p)) {
		return true;
	}
	return false;
}

std::vector<string_local> Application::StringSplit(const string_local& in, const string_local& delimit)
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
