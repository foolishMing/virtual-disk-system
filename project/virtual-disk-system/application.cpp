#include "application.h"
#include "./command/CommandArgs.h"
#include "./util/Banner.h"

Application::Application()
{
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
	assert(m_isCreate);
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
	assert(m_isCreate);
	Console::Write::Print(m_node_tree_manager.GetCurrentPath());
	Console::Write::Print(L">");
}


void Application::Run()
{
	assert(m_isCreate);
	if (!m_isCreate)
	{
		Log::LogError(L"Ӧ�ó���δ��ʼ��");
		return;
	}
	string_local input;
	while (true)
	{
		PrintCurrentPath();
		Console::Read::ReadLine(input);
		auto input_trim = StringTools::StringTrimed(input);
		//���Կմ�
		if (input_trim.length() == 0)
		{
			continue;
		}
		auto stat = ExecCommand(input_trim);
		//�˳�ѭ��
		if(Application::RunStatus::quit == stat)
		{
			break;
		}
	}
}

Application::RunStatus Application::ExecCommand(const string_local& in)
{
	assert(!in.empty());
	//��������,�õ�ָ�����args{cmd_type + options + paths}
	CommandArg arg;
	arg.Analyse(in);
	//��ȡ��������
	auto cmd_type = m_cmd_factory->GetCommandTypeByToken(arg.cmd_token);
	//�˳�Ӧ�ó���
	if (cmd_type == CommandType::quit) {
		return RunStatus::quit;
	}
	//��ȡ����ʵ��
	auto cmd_instance = m_cmd_factory->GetCommandInstance(cmd_type);
	//ִ������
	cmd_instance->Handle(arg, m_node_tree_manager);
	return RunStatus::normal;
}
