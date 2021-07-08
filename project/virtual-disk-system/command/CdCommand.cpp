#include "CdCommand.h"

CdCommand::CdCommand()
{

}

CdCommand::~CdCommand()
{

}

//-features
//切换工作目录
//-format
//cd path 
//-constraints:
//1、该命令只接受0或1个合法的path参数，如果有多余参数，则打印错误信息
//2、查找目标节点，若存在，且为目录节点，则切换工作目录
void CdCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//命令语法不正确
	if (arg.options.size() != 0 || arg.paths.size() > 1)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);
		return;
	}
	//显示工作目录
	if (arg.paths.empty())
	{
		node_tree_manager.ChangeDirByTokens({});
		return;
	}
	auto path = arg.paths[0];
	assert(!path.empty());
	//获取tokens
	auto map_item = arg.tokens_map.find(path);
	if (arg.tokens_map.end() == map_item)
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名称错误
		return;
	}
	//检查路径是否存在
	std::vector<string_local> tokens = map_item->second;	
	bool exist_path = node_tree_manager.IsPathExist(tokens);
	if (!exist_path)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
		return;
	}
	bool ret = node_tree_manager.ChangeDirByTokens(tokens);
	if (!ret)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : 路径不存在
	}
}
