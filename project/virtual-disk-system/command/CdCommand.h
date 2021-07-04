#pragma once
#include "BaseCommand.h"


class CdCommand : public BaseCommand
{
public:
	explicit CdCommand();
	~CdCommand();
	virtual void Handle(const std::vector<string_local>& args, NodeTreeManager& node_tree_manager);
};
