#include "LoadCommand.h"


LoadCommand::LoadCommand()
{

}

LoadCommand::~LoadCommand()
{

}

void LoadCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	//检查命令语法
	if (!arg.options.empty() || arg.paths.size() != 1)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	//检查路径是否合法
	Path path = arg.paths[0];
	string_local load_path_str = path.ToString();
	assert(!load_path_str.empty());
	if (load_path_str[0] != CharSet::char_at)
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal);//error : 磁盘路径格式不正确
		return;
	}
	load_path_str = load_path_str.substr(1, load_path_str.length() - 1);
	if (load_path_str.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal); //error : 不合法的磁盘路径名称
		return;
	}
	if (!PathTools::IsDiskPathExist(load_path_str))
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotFound); //error : 磁盘路径不存在
		return;
	}
	//判断文件后缀名是否为.xml
	auto xml_suffix_str = TEXT(".xml");
	if (load_path_str.find(xml_suffix_str) == load_path_str.npos)
	{
		Console::Write::PrintLine(TEXT("不是合法的XML格式文件，无法打开"));//error : 文件格式不正确
		return;
	}
	//读取XML，销毁原NodeTree,创建新NodeTree
	ReturnType ret = manager.LoadFromPath(load_path_str);
	if (ReturnType::LoadXmlFileFailed == ret)
	{
		Console::Write::PrintLine(TEXT("无法解析指定的XML文件"));
		return;
	}
}