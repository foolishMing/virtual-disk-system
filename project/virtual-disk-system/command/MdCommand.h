#pragma once
#include "BaseCommand.h"


class MdCommand : public BaseCommand
{
public:
	explicit MdCommand();
	~MdCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};