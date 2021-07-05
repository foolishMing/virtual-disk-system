#pragma once
#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "BaseNode.h"
#include "DirNode.h"
#include "SymlinkNode.h"
#include "../util/Common.h"

class NodeTree : Object {
public:
	explicit NodeTree();
	virtual	~NodeTree();
	virtual void Create();
	virtual void Destroy();
	BaseNode* GetRoot() { return m_root; }
	void InsertNode(BaseNode* node, BaseNode* new_child); //Ϊ�ڵ�����µ��ӽڵ�
	bool FindNode(std::vector<BaseNode*>& node_path_vec, std::vector<string_local>& name_list);
	//void DeleteNode(BaseNode* node);	//ɾ���ڵ�
	//void MoveNode(BaseNode* node, BaseNode* new_parent);	//Ϊ�ڵ�ָ���µĸ��ڵ�
	//BaseNode* FindNodeByAbsolutePath(std::vector<string_local>& absolute_path_list);
	
private:
	BaseNode* m_root = nullptr;			//�ڱ��ڵ�
};
#endif // !__NODETREE_H__
