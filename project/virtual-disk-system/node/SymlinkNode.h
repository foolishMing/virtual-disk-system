//<链接节点>
//成员属性：链接对象（指针）
#pragma once 
#ifndef __SYMLINKNODE_H__
#define __SYMLINKNODE_H__

#include "BaseNode.h"

class SymlinkNode : public BaseNode
{
public:
	explicit SymlinkNode(string_local name, BaseNode* parent = nullptr);
	virtual ~SymlinkNode();

	virtual const size_t GetSize() const; 
	virtual const time_t GetLatestModifiedTimeStamp(); //获取修改时间

	void SetSymlinkNode(NodeType link_type, BaseNode* dst_node); //设置链接对象
	BaseNode* GetSymlinkNode() const { return m_target_node; } //获取链接对象
	string_local GetSymlinkName()const { return m_target_name; } //获取链接对象名

private:
	BaseNode* m_target_node = nullptr;
	string_local m_target_name = TEXT("");
	virtual void SetLatestModifiedTimeStamp(time_t ts); //设置修改时间	
};

#endif // !__SYMLINKNODE_H__
