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
	//IO初始化
	Console::Init();
	//创建资源管理器
	m_node_tree_manager.Create();
	//创建命令工厂
	m_cmd_factory = new CommandFactory();
	m_cmd_factory->Create();
	PrintBanner();
	m_isCreate = true;
}

void Application::Destroy()
{
	assert(m_isCreate == true);
	//销毁命令工厂
	if (nullptr != m_cmd_factory)
	{
		m_cmd_factory->Destroy();
		m_cmd_factory = nullptr;
	}
	//销毁资源管理器
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
		Log::LogError(L"应用程序未初始化");
		return;
	}
	string_local input;
	while (true)
	{
		PrintCurrentPath();
		ReadLine(input);
		auto input_trim = StringTools::StringTrimed(input);
		//忽略空串
		if (input_trim.length() == 0)
		{
			continue;
		}
		auto stat = ExecCommand(input_trim);
		//退出循环
		if(Application::RunStatus::quit == stat)
		{
			break;
		}
	}
}

/*
* 1、in -> 
* 2、args -> cmd_instance
* 3、cmd_instance->exec()
*/
Application::RunStatus Application::ExecCommand(const string_local& in)
{
	assert(0 != in.length());
	//解析输入字串得到指令参数args{cmd_type + options + paths}
	CommandArg arg;
	arg.Analyse(in);

	//获取命令类型
	auto lowercase_cmd_token = arg.GetLowercaseToken();
	auto cmd_type = m_cmd_factory->GetCommandTypeByToken(lowercase_cmd_token);
	//退出应用程序
	if (cmd_type == CommandType::quit) {
		return RunStatus::quit;
	}
	//获取命令实例
	auto cmd_instance = m_cmd_factory->GetCommandInstance(cmd_type);
	//执行命令
	cmd_instance->Handle(arg, m_node_tree_manager);
	return RunStatus::normal;
}






