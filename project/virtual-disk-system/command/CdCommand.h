#pragma once
#ifndef __CDCOMMAND_H__
#define __CDCOMMAND_H__

#include "BaseCommand.h"
class CdCommand : public BaseCommand
{
public:
	explicit CdCommand();
	~CdCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__CDCOMMAND_H__



