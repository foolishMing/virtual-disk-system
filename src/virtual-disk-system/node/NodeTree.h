#pragma once
#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "BaseNode.h"
#include "../util/Common.h"

class NodeTree : Object {
public:
	explicit NodeTree();
	virtual	~NodeTree();
	virtual void Create();
	virtual void Destroy();
	void InsertNode(BaseNode* node, BaseNode* new_child); //为节点添加新的子节点
	void DeleteNode(BaseNode* node);	//删除节点
	void MoveNode(BaseNode* node, BaseNode* new_parent);	//为节点指定新的父节点
	BaseNode* FindNodeByAbsolutePath(std::vector<string_local>& absolute_path_list);
	

private:
	BaseNode* m_root = nullptr;			//哨兵节点

};
#endif // !__NODETREE_H__
