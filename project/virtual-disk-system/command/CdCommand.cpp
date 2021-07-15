#include "CdCommand.h"
#include "../util/Path.h"

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
	//获取tokens
	Path path = arg.paths[0];
	if (!path.IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名称错误
		return;
	}
	auto tokens = path.Tokens();
	//切换路径
	const ReturnType ret = node_tree_manager.ChangeDirByTokens(tokens);
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : 找不到路径
		return;
	}
}
