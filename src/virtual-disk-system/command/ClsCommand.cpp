#include "ClsCommand.h"
#include "../util/Banner.h"

ClsCommand::ClsCommand()
{

}

ClsCommand::~ClsCommand()
{

}

void ClsCommand::Handle(std::vector<string_local>& args, NodeTreeProxy& proxy)
{
	system("cls");
	PrintBanner();
}