#pragma once
#ifndef __SAVECOMMAND_H__
#define __SAVECOMMAND_H__

#include "BaseCommand.h"

class SaveCommand : public BaseCommand
{
public:
	explicit SaveCommand();
	~SaveCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__SAVECOMMAND_H__


