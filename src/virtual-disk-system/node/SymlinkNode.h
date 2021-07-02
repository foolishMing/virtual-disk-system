#pragma once 
#ifndef __SYMLINKNODE_H__
#define __SYMLINKNODE_H__

#include "BaseNode.h"

class SymlinkNode : public BaseNode
{
public:
	explicit SymlinkNode(string_local& name, BaseNode* parent = nullptr);
	virtual ~SymlinkNode();
	void SetSymlink(NodeType type, BaseNode* dst_node);	//设置链接对象
	virtual uint64_t GetSize();
private:
	BaseNode* m_dst_node = nullptr;
};

#endif // !__SYMLINKNODE_H__
