#pragma once
#include "BaseCommand.h"


class DelCommand : public BaseCommand
{
public:
	explicit DelCommand();
	~DelCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};