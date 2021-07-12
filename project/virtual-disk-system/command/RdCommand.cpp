#include "RdCommand.h"

RdCommand::RdCommand()
{

}

RdCommand::~RdCommand()
{

}


//-features
//删除目录
//-format
//rd [/s] path [path1] [path2] ...
//-constraints
//1、工作目录中的结点不能被删除
//2、仅支持/s参数
//3、path列表不能为空
void RdCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//检查选项列表，支持/s选项
	OptionSwitch option_switch;
	for (auto opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_s))// /s
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + TEXT(" ") + opt);//error : 无效的开关
			return;
		}
	}
	//检查路径列表，列表不能为空
	if (arg.paths.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	//遍历path列表
	for (const auto& path : arg.paths)
	{
		auto path_str = path.ToString();
		//获得tokens
		if (!path.IsValid())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名称语法错误
			continue;
		}
		auto tokens = path.Tokens();
		//检查路径是否存在
		bool exist_path = node_tree_manager.IsPathExist(tokens);
		if (!exist_path)
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : 系统找不到指定的虚拟磁盘路径
			return;
		}
		//删除目录
		ReturnType ret = node_tree_manager.RemoveDirByTokensAndOptions(tokens, option_switch);
		switch (ret)
		{
		case ReturnType::Success:
			Console::Write::PrintLine(TEXT("删除路径 ") + path_str + TEXT(" 成功"));
			break;
		case ReturnType::UnExpectedException:
			Console::Write::PrintLine(TEXT("删除路径 ") + path_str + TEXT(" 时发生了未预期的错误"));
				Log::LogError(TEXT("删除路径 ") + path_str + TEXT(" 时发生了未预期的错误"));
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