#include "ClsCommand.h"
#include "../util/Banner.h"

ClsCommand::ClsCommand()
{

}

ClsCommand::~ClsCommand()
{

}

void ClsCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//检查命令格式
	if (!arg.paths.empty() || !arg.options.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//命令语法不正确
		return;
	}
	//清屏
	system("cls");
	PrintBanner();
}