#pragma once
#include "BaseCommand.h"


class LoadCommand : public BaseCommand
{
public:
	explicit LoadCommand();
	~LoadCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};
