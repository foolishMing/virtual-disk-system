#pragma once
#include "BaseCommand.h"


class MklinkCommand : public BaseCommand
{
public:
	explicit MklinkCommand();
	~MklinkCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};