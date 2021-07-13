//<目录节点>
#pragma once 
#ifndef  __DIRNODE_H__
#define __DIRNODE_H__

#include "BaseNode.h"

class DirNode : public BaseNode {
public:
	explicit DirNode(string_local name, BaseNode* parent = nullptr);
	virtual ~DirNode();

	virtual const size_t GetSize() const;
	virtual const time_t GetLatestModifiedTimeStamp(); //获取修改时间

	BaseNode* FindChildByName(const string_local& node_name); //根据节点名称获取子节点
	bool ContainsChild(const string_local& node_name); //根据节点名称判断子节点是否存在
	void AppendChild(BaseNode* node); //向Children列表末尾追加(非空)子节点
	bool RemoveChildByIndex(int index); //删除第index个儿子节点
	std::vector<BaseNode*> Children() { return m_children; }

private:
	std::vector<BaseNode*> m_children = {};
	virtual void SetLatestModifiedTimeStamp(time_t ts); //设置修改时间	
};

#endif // ! __DIRNODE_H__