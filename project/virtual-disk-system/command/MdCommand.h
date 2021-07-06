#pragma once
#ifndef __MDCOMMAND_H__
#define __MDCOMMAND_H__

#include "BaseCommand.h"

class MdCommand : public BaseCommand
{
public:
	explicit MdCommand();
	~MdCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__MDCOMMAND_H__

