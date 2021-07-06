#pragma once
#ifndef __LOADCOMMAND_H__
#define __LOADCOMMAND_H__

#include "BaseCommand.h"

class LoadCommand : public BaseCommand
{
public:
	explicit LoadCommand();
	~LoadCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__LOADCOMMAND_H__


