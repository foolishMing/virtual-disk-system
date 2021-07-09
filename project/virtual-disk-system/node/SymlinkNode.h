#pragma once 
#ifndef __SYMLINKNODE_H__
#define __SYMLINKNODE_H__

#include "BaseNode.h"

class SymlinkNode : public BaseNode
{
public:
	explicit SymlinkNode(string_local name, BaseNode* parent = nullptr);
	virtual ~SymlinkNode();

	virtual size_t GetSize(); 
	virtual const time_t GetLatestModifiedTimeStamp(); //获取修改时间

	void SetSymlinkNode(NodeType link_type, BaseNode* dst_node); //设置链接对象
	BaseNode* GetSymlinkNode() const { return m_dst_node; } //获取链接对象

private:
	BaseNode* m_dst_node = nullptr;
	virtual void SetLatestModifiedTimeStamp(time_t ts); //设置修改时间	
};

#endif // !__SYMLINKNODE_H__
