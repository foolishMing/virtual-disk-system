//<文件管理树>
//使用多叉树的形式管理文件和目录
//目录、文件、链接都是树上的节点
//树上的每个中间节点都一定是目录节点，每个目录节点都维护一个Children列表，指向其下一级路径
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
