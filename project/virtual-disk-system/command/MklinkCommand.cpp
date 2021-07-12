#include "MklinkCommand.h"


MklinkCommand::MklinkCommand()
{

}

MklinkCommand::~MklinkCommand()
{

}

void MklinkCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	OptionSwitch option_switch;
	//��������ʽ
	for (const auto& item : arg.options)
	{
		if (StringTools::IsEqual(item, Constant::gs_option_d))
		{
			option_switch._d = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
			return;
		}
	}
	if (arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	if (!arg.paths[0].IsValid() || !arg.paths[1].IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsIllegal);//error : �ļ���Ŀ¼�������﷨����ȷ
		return;
	}
	//��������·���Ƿ����
	auto linkedPath = arg.paths[1];
	const auto src_path_tokens = linkedPath.Tokens();
	{
		bool exist_path = manager.IsPathExist(src_path_tokens);
		if (!exist_path)//error : ����·��������
		{
			Console::Write::PrintLine(TEXT("����Ŀ¼���ļ� ") + linkedPath.ToString() + TEXT(" ������"));
			return;
		}
	}
	//����ݷ�ʽ�Ƿ��Ѵ���
	auto linkPath = arg.paths[0];
	const auto symbol_path_tokens = linkPath.Tokens();
	{
		bool exist_path = manager.IsPathExist(symbol_path_tokens);
		if (exist_path)//error : ��ݷ�ʽ�Ѵ���
		{
			Console::Write::PrintLine(TEXT("��ݷ�ʽ ") + linkPath.ToString() + TEXT(" �Ѵ���"));
			return;
		}
	}
	//Ϊ������·��������ݷ�ʽ
	ReturnType ret = manager.MklinkByTokensAndOptions(symbol_path_tokens, src_path_tokens, option_switch);
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
		return;
	}
	if (ret == ReturnType::MemoryPathNameIsIllegal)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsIllegal);
		return;
	}
}
