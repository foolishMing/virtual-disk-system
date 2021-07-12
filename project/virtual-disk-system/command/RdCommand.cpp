#include "RdCommand.h"

RdCommand::RdCommand()
{

}

RdCommand::~RdCommand()
{

}


//-features
//ɾ��Ŀ¼
//-format
//rd [/s] path [path1] [path2] ...
//-constraints
//1������Ŀ¼�еĽ�㲻�ܱ�ɾ��
//2����֧��/s����
//3��path�б���Ϊ��
void RdCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//���ѡ���б�֧��/sѡ��
	OptionSwitch option_switch;
	for (auto opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_s))// /s
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + TEXT(" ") + opt);//error : ��Ч�Ŀ���
			return;
		}
	}
	//���·���б��б���Ϊ��
	if (arg.paths.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	//����path�б�
	for (const auto& path : arg.paths)
	{
		auto path_str = path.ToString();
		//���tokens
		if (!path.IsValid())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼��������﷨����
			continue;
		}
		auto tokens = path.Tokens();
		//���·���Ƿ����
		bool exist_path = node_tree_manager.IsPathExist(tokens);
		if (!exist_path)
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : ϵͳ�Ҳ���ָ�����������·��
			return;
		}
		//ɾ��Ŀ¼
		ReturnType ret = node_tree_manager.RemoveDirByTokensAndOptions(tokens, option_switch);
		switch (ret)
		{
		case ReturnType::Success:
			Console::Write::PrintLine(TEXT("ɾ��·�� ") + path_str + TEXT(" �ɹ�"));
			break;
		case ReturnType::UnExpectedException:
			Console::Write::PrintLine(TEXT("ɾ��·�� ") + path_str + TEXT(" ʱ������δԤ�ڵĴ���"));
				Log::LogError(TEXT("ɾ��·�� ") + path_str + TEXT(" ʱ������δԤ�ڵĴ���"));
			break;
		case ReturnType::DirNameIsInvalid:
			Console::Write::PrintLine(ErrorTips::gsDirNameInvalid);
			break;
		case ReturnType::AccessDenied:
			Console::Write::PrintLine(ErrorTips::gsMemoryPathAccessDenied);
			break;
		case ReturnType::MemoryDirIsNotEmpty:
			Console::Write::PrintLine(ErrorTips::gsMemoryDirIsNotEmpty);
			break;
		default:
			break;
		}
	}
}