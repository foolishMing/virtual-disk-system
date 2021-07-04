#include "ClsCommand.h"
#include "../util/Banner.h"

ClsCommand::ClsCommand()
{

}

ClsCommand::~ClsCommand()
{

}

void ClsCommand::Handle(std::vector<string_local>& args, NodeTreeManager& proxy)
{
	system("cls");
	PrintBanner();
}