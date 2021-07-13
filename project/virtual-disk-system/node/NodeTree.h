//<�ļ�������>
//ʹ�ö��������ʽ�����ļ���Ŀ¼
//Ŀ¼���ļ������Ӷ������ϵĽڵ�
//���ϵ�ÿ���м�ڵ㶼һ����Ŀ¼�ڵ㣬ÿ��Ŀ¼�ڵ㶼ά��һ��Children�б�ָ������һ��·��
#pragma once
#ifndef __NODETREE_H__
#define __NODETREE_H__

#include "BaseNode.h"
#include "DirNode.h"
#include "SymlinkNode.h"
#include "../util/Common.h"
#include <deque>

//�����
class NodeTree : Object {
public:
	explicit NodeTree();
	virtual	~NodeTree();
	virtual void Create();
	virtual void Destroy();
	BaseNode* GetRoot() { return m_root; }

	//Ϊ�ڵ�����µ��ӽڵ�
	bool InsertNode(BaseNode* node, BaseNode* new_child); 
	
	//ɾ���ڵ�
	bool DeleteNode(BaseNode* node); 

	//������ӹ�ϵ������ɾ���ڵ�
	bool RemoveButNotDeleteNode(BaseNode* node);
private:
	BaseNode* m_root = nullptr;	//���ڵ�
};
#endif // !__NODETREE_H__
