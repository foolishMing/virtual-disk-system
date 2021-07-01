#pragma once
#ifndef __BASENODE_H__
#define __BASENODE_H__

#include "../util/Common.h"
#include "../util/Name.h"

interface IBaseNode {

};

//节点类型
enum NodeType {
	Directory,		//目录
	File,			//文件
	SymlinkD,		//快捷方式(目录)
	SymlinkF		//快捷方式(文件)	
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
