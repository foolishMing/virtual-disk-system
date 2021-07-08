#include "MoveCommand.h"


MoveCommand::MoveCommand()
{

}

MoveCommand::~MoveCommand()
{

}

//-constraints
//1、文件可以被覆盖，目录不可以——error ： 拒绝访问
void MoveCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
{

}