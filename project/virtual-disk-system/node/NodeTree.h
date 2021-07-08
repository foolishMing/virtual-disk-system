#pragma once
#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "BaseNode.h"
#include "DirNode.h"
#include "SymlinkNode.h"
#include "../util/Common.h"
#include <deque>

//多叉树
class NodeTree : Object {
public:
	explicit NodeTree();
	virtual	~NodeTree();
	virtual void Create();
	virtual void Destroy();
	BaseNode* GetRoot() { return m_root; }

	//为节点添加新的子节点
	bool InsertNode(BaseNode* node, BaseNode* new_child); 
	
	//删除节点
	bool DeleteNode(BaseNode* node); 

	
	//解除父子关系，但不删除节点
	bool RemoveButNotDeleteNode(BaseNode* node);
private:
	BaseNode* m_root = nullptr;	//根节点
};
#endif // !__NODETREE_H__
