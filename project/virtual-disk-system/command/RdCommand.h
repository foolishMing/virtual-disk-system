#pragma once
#include "BaseCommand.h"


class RdCommand : public BaseCommand
{
public:
	explicit RdCommand();
	~RdCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};