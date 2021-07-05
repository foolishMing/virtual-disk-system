#pragma once
#include "BaseCommand.h"


class DirCommand : public BaseCommand
{
public:
	explicit DirCommand();
	~DirCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};