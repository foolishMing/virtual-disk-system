#pragma once
#include "BaseCommand.h"

class UndefinedCommand : public BaseCommand
{
public:
	explicit UndefinedCommand();
	virtual ~UndefinedCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};
