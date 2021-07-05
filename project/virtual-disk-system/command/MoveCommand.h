#pragma once
#include "BaseCommand.h"


class MoveCommand : public BaseCommand
{
public:
	explicit MoveCommand();
	~MoveCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};