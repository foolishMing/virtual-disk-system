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
	//��������ʽ
	if (!arg.paths.empty() || !arg.options.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//�����﷨����ȷ
		return;
	}
	//����
	system("cls");
	PrintBanner();
}