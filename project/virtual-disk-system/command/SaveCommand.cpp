#include "SaveCommand.h"


SaveCommand::SaveCommand()
{

}

SaveCommand::~SaveCommand()
{

}

void SaveCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	//检查命令语法
	if (!arg.options.empty() || arg.paths.size() != 1)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	//检查路径是否合法
	Path path = arg.paths[0];
	string_local save_path_str = path.ToString();
	assert(!save_path_str.empty());
	if (save_path_str[0] != CharSet::char_at)
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal);//error : 磁盘路径格式不正确
		return;
	}
	save_path_str = save_path_str.substr(1, save_path_str.length() - 1);
	if (save_path_str.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal); //error : 不合法的磁盘路径名称
		return;
	}
	if (PathTools::IsDiskPathExist(save_path_str))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist); //error : 磁盘路径已存在
		return;
	}
	//如果没有后缀名就补上
	auto xml_suffix_str = TEXT(".xml");
	if (save_path_str.find(xml_suffix_str) == save_path_str.npos)
	{
		save_path_str.append(xml_suffix_str);
	}
	//将当前NodeTree保存到XML文件中
	ReturnType ret = manager.SaveToPath(save_path_str);
}