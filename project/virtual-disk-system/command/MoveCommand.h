#pragma once
#ifndef __MOVECOMMAND_H__
#define __MOVECOMMAND_H__

#include "BaseCommand.h"

class MoveCommand : public BaseCommand
{
public:
	explicit MoveCommand();
	~MoveCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__MOVECOMMAND_H__

