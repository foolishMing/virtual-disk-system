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
	//解析选项列表
	OptionSwitch option_switch;
	for (auto item : arg.options)
	{
		const string_local lc_option = StringTools::ToLowercase(item);
		if (lc_option == Constant::gs_option_s)// /s
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + item);//error : 无效的开关
			return;
		}
	}
	//path列表不能为空
	if (arg.paths.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	//遍历path列表
	for (auto path : arg.paths)
	{
		//获得tokens
		auto map_item = arg.tokens_map.find(path);
		if (arg.tokens_map.end() == map_item)
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名称语法错误
			continue;
		}
		//检查路径是否存在
		std::vector<string_local> tokens = map_item->second;
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
			Console::Write::PrintLine(L"删除路径 " + path + L" 成功");
			break;
		case ReturnType::UnExpectedException:
			Console::Write::PrintLine(L"删除路径 " + path + L" 时发生了未预期的错误");
			Log::LogError(L"删除路径 " + path + L" 时发生了未预期的错误");
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