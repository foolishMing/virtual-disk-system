#include "RenCommand.h"

RenCommand::RenCommand()
{

}

RenCommand::~RenCommand()
{

}


//-features
//重命名
//-format
//ren src dst
//-constrains
//1、判断源文件是否存在
//2、判断新名字是否合法、重名
void RenCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//命令语法不正确
	if (arg.options.size() != 0 || arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);
		return;
	}
	assert(!arg.paths[0].empty() && !arg.paths[1].empty());//路径不能为空
}

