#include "UndefinedCommand.h"
#include "../util/Console.hpp"

UndefinedCommand::UndefinedCommand()
{

}

UndefinedCommand::~UndefinedCommand()
{

}

void UndefinedCommand::Handle(std::vector<string_local>& args, NodeTreeProxy& proxy)
{
	Console::Write::PrintLine(Tips::gsUndefinedCommand);	//ÎŞĞ§µÄÃüÁî
}