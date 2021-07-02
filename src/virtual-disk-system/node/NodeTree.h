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
	void InsertNode(BaseNode* node, BaseNode* new_child); //Ϊ�ڵ�����µ��ӽڵ�
	void DeleteNode(BaseNode* node);	//ɾ���ڵ�
	void MoveNode(BaseNode* node, BaseNode* new_parent);	//Ϊ�ڵ�ָ���µĸ��ڵ�
	BaseNode* FindNodeByAbsolutePath(std::vector<string_local>& absolute_path_list);
	

private:
	BaseNode* m_root = nullptr;			//�ڱ��ڵ�

};
#endif // !__NODETREE_H__
