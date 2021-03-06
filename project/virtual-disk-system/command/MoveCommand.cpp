#include "MoveCommand.h"


MoveCommand::MoveCommand()
{

}

MoveCommand::~MoveCommand()
{

}

//-format
//move /y src dst
//-constraints
//1、若重名，文件可以被覆盖，目录不可以——error ： 拒绝访问
//2、工作路径上的节点不允许move
//3、源路径必须存在；目标路径可以不存在，但必须与源路径在同一目录下，此时相当于重命名
//4、暂时不考虑对通配符的支持
void MoveCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	OptionSwitch option_switch;
	//命令语法不正确
	for (const auto& opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_y))
		{
			option_switch._y = true;
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
	auto src_path = arg.paths[0], dst_path = arg.paths[1];
	if (!src_path.IsValid() || !dst_path.IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error ： 文件、目录或卷名称错误
		return;
	}
	const auto src_path_tokens = src_path.Tokens();
	const auto dst_path_tokens = dst_path.Tokens();
	if (src_path_tokens.empty() || dst_path_tokens.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : 路径不存在 
		return;
	}
	//move from src_path to dst_path
	ReturnType ret = manager.MoveByTokensAndOptions(src_path, dst_path, option_switch);
	if (ret == ReturnType::AccessDenied)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathAccessDenied);//error ：虚拟磁盘拒绝访问
	}
	else if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(TEXT("目录或文件 ") + src_path.ToString() + TEXT(" 不存在"));
	}
	else if (ret == ReturnType::MemoryPathNameIsIllegal)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsIllegal);
	}
}












