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
	system("cls");
	PrintBanner();
}