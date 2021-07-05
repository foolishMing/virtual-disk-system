#include "application.h"
#include "./command/CommandArgs.h"
#include "./util/Banner.h"

Application::Application()
{
	//spdlog::error("...");
	//spdlog::critical("...");
}

Application::~Application()
{

}

void Application::Create()
{
	//IO��ʼ��
	Console::Init();
	//������Դ������
	m_node_tree_manager.Create();
	//���������
	m_cmd_factory = new CommandFactory();
	m_cmd_factory->Create();
	PrintBanner();
	m_isCreate = true;
}

void Application::Destroy()
{
	assert(m_isCreate == true);
	//���������
	if (nullptr != m_cmd_factory)
	{
		m_cmd_factory->Destroy();
		m_cmd_factory = nullptr;
	}
	//������Դ������
	m_node_tree_manager.Destroy();	
}

void Application::PrintCurrentPath()
{
	assert(m_isCreate == true);
	Console::Write::Print(m_node_tree_manager.GetCurrentPath());
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
		Log::LogError(L"Ӧ�ó���δ��ʼ��");
		return;
	}
	typedef Application::RunStatus stat;
	string_local input;
	while (true)
	{
		PrintCurrentPath();
		ReadLine(input);
		input = StringTools::StringTrimed(input);
		//���Կմ�
		if (input.length() == 0)
		{
			continue;
		}
		auto stat = ExecCommand(input);
		//��������
		if(stat::exit == stat)	
		{
			return;
		}
	}
}

/*
* 1��in -> args{cmd_type + options + paths}
* 2��args -> cmd_instance
* 3��cmd_instance->exec()
*/
Application::RunStatus Application::ExecCommand(const string_local& in)
{
	assert(0 != in.length());	//���봮���Ȳ���Ϊ0
	//std::vector<string_local> delimits{ L" " };
	//���������ִ��õ�ָ�����
	CommandArg arg;
	arg.Analyse(in);
	//��ȡ��������
	auto lowercase_cmd_token = arg.GetLowercaseToken();
	auto cmd_type = m_cmd_factory->GetCommandTypeByToken(lowercase_cmd_token);
	//�˳�Ӧ�ó���
	if (cmd_type == CommandType::quit) {
		return RunStatus::exit;
	}
	//��ȡ����ʵ��
	auto cmd_instance = m_cmd_factory->GetCommandInstance(cmd_type);
	//ִ������
	cmd_instance->Handle(arg, m_node_tree_manager);
	return RunStatus::normal;
}






