#pragma once
#ifndef __DELCOMMAND_H__
#define __DELCOMMAND_H__

#include "BaseCommand.h"

class DelCommand : public BaseCommand
{
public:
	explicit DelCommand();
	~DelCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__DELCOMMAND_H__

