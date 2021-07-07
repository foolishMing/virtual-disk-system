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

}

