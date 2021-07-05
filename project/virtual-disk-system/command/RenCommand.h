#pragma once
#include "BaseCommand.h"

class RenCommand : public BaseCommand
{
public:
	explicit RenCommand();
	~RenCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};