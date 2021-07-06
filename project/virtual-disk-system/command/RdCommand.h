#pragma once
#ifndef __RDCOMMAND_H__
#define __RDCOMMAND_H__

#include "BaseCommand.h"

class RdCommand : public BaseCommand
{
public:
	explicit RdCommand();
	~RdCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__RDCOMMAND_H__

