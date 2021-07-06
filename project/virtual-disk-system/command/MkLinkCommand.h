#pragma once
#ifndef __MKLINKCOMMAND_H__
#define __MKLINKCOMMAND_H__

#include "BaseCommand.h"

class MklinkCommand : public BaseCommand
{
public:
	explicit MklinkCommand();
	~MklinkCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__MKLINKCOMMAND_H__

