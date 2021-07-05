#pragma once
#include "BaseCommand.h"


class CdCommand : public BaseCommand
{
public:
	explicit CdCommand();
	~CdCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};
