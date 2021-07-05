#pragma once
#include "BaseCommand.h"

class SaveCommand : public BaseCommand
{
public:
	explicit SaveCommand();
	~SaveCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};