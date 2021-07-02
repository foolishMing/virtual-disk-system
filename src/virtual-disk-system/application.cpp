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
	//�����ļ���������
	m_node_tree_manager = new NodeTreeManager();
	m_node_tree_manager->Create();
	//���������
	m_command_factory = new CommandFactory();
	m_command_factory->Create();
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
	if (nullptr != m_node_tree_manager)
	{
		m_node_tree_manager->Destroy();
		m_node_tree_manager = nullptr;
	}
}

void Application::PrintCurrentPath()
{
	assert(m_isCreate == true);
	Console::Write::Print(m_node_tree_manager->GetCurrentPath());
	Console::Write::Print(L">");
}

void Application::ReadLine(string_local& input)
{
	Console::Read::ReadLine(input);
}

void Application::Run()
{
	assert(m_isCreate == true);
	if (!m_isCreate)
	{
		Console::Write::PrintLine(L"Ӧ�ó���δ��ʼ��");
		return;
	}
	typedef Application::RunStatus stat;
	string_local input;
	while (true)
	{
		PrintCurrentPath();
		ReadLine(input);
		if(stat::exit == ExecCommand(input))	
		{
			return;
		}
	}
}

Application::RunStatus Application::ExecCommand(const string_local& strCmd)
{
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

//update : ������
std::vector<string_local> Application::StringSplits(const string_local& in, const std::vector<string_local>& delimits)
{
	std::vector<string_local> vec;
	string_local tmp = in;
	size_t nPos = string_local::npos;
	string_local cur_delimit;
	for (auto item : delimits)
	{
		auto next_pos = in.find(item.c_str());
		if (next_pos < nPos)
		{
			nPos = next_pos;
			cur_delimit = item;
		}
	}
	while (string_local::npos != nPos)
	{
		string_local item = tmp.substr(0, nPos);
		vec.push_back(item);
		tmp = tmp.substr(nPos + cur_delimit.length());
		nPos = tmp.find(cur_delimit.c_str());
	}
	return vec;
}
