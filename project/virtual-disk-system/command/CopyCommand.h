#pragma once

#ifndef __COPYCOMMAND_H__
#define __COPYCOMMAND_H__

#include "BaseCommand.h"

class CopyCommand : public BaseCommand
{
public:
	explicit CopyCommand();
	~CopyCommand();
	virtual void Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager);
};

#endif // !__COPYCOMMAND_H__


