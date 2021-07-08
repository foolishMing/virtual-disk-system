#pragma once 
#ifndef  __DIRNODE_H__
#define __DIRNODE_H__

#include "BaseNode.h"

class DirNode : public BaseNode {
public:
	explicit DirNode(string_local name, BaseNode* parent = nullptr);
	virtual ~DirNode();
	BaseNode* FindChildByName(const string_local& node_name);	//根据节点名称获取子节点
	bool ContainsChild(const string_local& node_name);	//根据节点名称判断子节点是否存在
	void DeleteChildByName(const string_local& node_name);	//根据节点名称删除子节点
	void DisposeChildByName(const string_local& node_name);	//根据节点名称移除该节点(不删除)
	void AppendChild(BaseNode* node);	//向Children列表末尾追加(非空)子节点
	virtual size_t GetSize();
	//删除第index个儿子节点
	void RemoveChildByIndex(int index);
	std::vector<BaseNode*> Children() { return m_children; }

private:
	std::vector<BaseNode*> m_children = {};
};

#endif // ! __DIRNODE_H__