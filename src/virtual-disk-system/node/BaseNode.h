#pragma once
#ifndef __BASENODE_H__
#define __BASENODE_H__

#include "../util/Common.h"
#include "../util/Name.h"

interface IBaseNode {

};

//�ڵ�����
enum NodeType {
	Directory,		//Ŀ¼
	File,			//�ļ�
	SymlinkD,		//��ݷ�ʽ(Ŀ¼)
	SymlinkF		//��ݷ�ʽ(�ļ�)	
};

class BaseNode : IBaseNode {
public:
	explicit BaseNode();
	explicit BaseNode(Name& name);
	virtual ~BaseNode();
protected:
	uint64_t size;						
	Name m_name;
	string_local data;
	BaseNode* m_parent = nullptr;
	std::vector<BaseNode*> children;
};

#endif // !__BASENODE_H__
