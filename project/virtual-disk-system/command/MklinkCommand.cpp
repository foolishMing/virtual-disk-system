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
	//Console::Write::PrintLine(TEXT("链接目录或文件 ") + srcPath.ToString() + TEXT(" 不存在"));
	//检查软链接是否已存在
	//Console::Write::PrintLine(TEXT("快捷方式 ") + linkPath.ToString() + TEXT(" 已存在"));
	auto linkPath = arg.paths[0];
	auto srcPath = arg.paths[1];
	//为待链接路径创建软链接
	ReturnType ret = manager.MklinkByPathAndOptions(linkPath, srcPath, option_switch);
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : 路径不存在
	}
	else if (ret == ReturnType::MemoryPathNameIsIllegal)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsIllegal);//error : 文件名不合法
	}
	else if (ret == ReturnType::TypeOfLinkAndSourceIsNotMatch)
	{
		Console::Write::PrintLine(ErrorTips::gsTypeOfLinkAndSourceIsNotMatch);//error : 链接与链接对象类型不匹配
	}
	else if (ret == ReturnType::MemoryFileIsExist)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist);//error : 重名文件已存在
	}
}
