#pragma once

#ifndef __UNDEFINEDCOMMAND_H__
#define __UNDEFINEDCOMMAND_H__

#include "BaseCommand.h"

class UndefinedCommand : public BaseCommand
{
public:
	explicit UndefinedCommand();
	virtual ~UndefinedCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__UNDEFINEDCOMMAND_H__







