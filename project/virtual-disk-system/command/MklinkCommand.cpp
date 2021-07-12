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
	//检查命令格式
	for (const auto& item : arg.options)
	{
		if (StringTools::IsEqual(item, Constant::gs_option_d))
		{
			option_switch._d = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
			return;
		}
	}
	if (arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	if (!arg.paths[0].IsValid() || !arg.paths[1].IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsIllegal);//error : 文件、目录卷名称语法不正确
		return;
	}
	//检查待链接路径是否存在
	auto linkedPath = arg.paths[1];
	const auto src_path_tokens = linkedPath.Tokens();
	{
		bool exist_path = manager.IsPathExist(src_path_tokens);
		if (!exist_path)//error : 链接路径不存在
		{
			Console::Write::PrintLine(TEXT("链接目录或文件 ") + linkedPath.ToString() + TEXT(" 不存在"));
			return;
		}
	}
	//检查快捷方式是否已存在
	auto linkPath = arg.paths[0];
	const auto symbol_path_tokens = linkPath.Tokens();
	{
		bool exist_path = manager.IsPathExist(symbol_path_tokens);
		if (exist_path)//error : 快捷方式已存在
		{
			Console::Write::PrintLine(TEXT("快捷方式 ") + linkPath.ToString() + TEXT(" 已存在"));
			return;
		}
	}
	//为待链接路径创建快捷方式
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
