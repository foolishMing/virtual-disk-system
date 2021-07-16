#include "DelCommand.h"

DelCommand::DelCommand()
{

}

DelCommand::~DelCommand()
{

}


//-feature
//删除文件
//-format
//del [/s] path [path1] ...
//-constraints
//1、若path表示一个目录，则删除该目录下的所有文件
//2、若path表示一个文件，则删除该文件
//3、当前版本暂时不支持通配符
void DelCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	//检查命令语法
	OptionSwitch option_switch;
	for (const auto& item : arg.options)
	{
		if (StringTools::IsEqual(item, Constant::gs_option_s))
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
			return;
		}
	}
	if (arg.paths.size() == 0)
	{
		//paths.emplace_back(Path{ Constant::gs_cur_dir_token });
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	for (const auto& path : arg.paths)
	{
		if (!path.IsValid())
		{
			Console::Write::PrintLine(path.ToString() + TEXT(" ") + ErrorTips::gsTokenNameIsIllegal);//error : 目录名、文件名或卷名语法不正确
			continue;
		}
		//删除文件
		//这里因为token中可能存在通配符，所以需要把path传进下层校验
		auto ret = manager.DelByTokensAndOption(path, option_switch);
		if (ret == ReturnType::MemoryPathIsNotFound)
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
		}
		else if (ret == ReturnType::SymlinkPathIsInvalid)
		{
			Console::Write::PrintLine(ErrorTips::gsSymlinkPathIsInvalid);
		}
	}
}










