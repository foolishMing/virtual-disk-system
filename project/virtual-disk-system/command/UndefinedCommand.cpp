#include "UndefinedCommand.h"
#include "../util/Console.hpp"

UndefinedCommand::UndefinedCommand()
{

}

UndefinedCommand::~UndefinedCommand()
{

}

void UndefinedCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
{
	Console::Write::PrintLine(Tips::gsCommandIsNotFound);	//ÎŞĞ§µÄÃüÁî
}