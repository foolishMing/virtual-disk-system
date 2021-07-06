#pragma once
#ifndef __DIRCOMMAND_H__
#define __DIRCOMMAND_H__

#include "BaseCommand.h"

class DirCommand : public BaseCommand
{
public:
	explicit DirCommand();
	~DirCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__DIRCOMMAND_H__

