#pragma once
#ifndef BASENODE_H
#define BASENODE_H

#include "../util/Common.h"
#include "../util/Name.h"

interface IBaseNode {

};

//节点类型
enum NodeType {
	Directory,
	File
};

class BaseNode : IBaseNode {
public:
	explicit BaseNode();
	explicit BaseNode(Name& name);
	virtual ~BaseNode();
protected:
	Name m_name;
	BaseNode* m_parent = nullptr;
	std::vector<BaseNode*> children;
};

#endif // !BASENODE_H
