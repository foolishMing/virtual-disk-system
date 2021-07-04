#pragma once
#ifndef __BASECOMMAND_H__
#define __BASECOMMAND_H__

#include "../util/common.h"
#include "../node/NodeTreeManager.h"




interface IBaseCommand
{
	virtual void Handle() = 0;
};

class BaseCommand : IBaseCommand, Object{
public:
	explicit BaseCommand();
	~BaseCommand();
	virtual void Handle();
	virtual void Handle(const std::vector<string_local>& args, NodeTreeManager& node_tree_manager);
	virtual void Create();
	virtual void Destroy();
};

#endif // !__BASECOMMAND_H__

