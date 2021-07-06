#pragma once
#ifndef __RENCOMMAND_H__
#define __RENCOMMAND_H__

#include "BaseCommand.h"

class RenCommand : public BaseCommand
{
public:
	explicit RenCommand();
	~RenCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__RENCOMMAND_H__

