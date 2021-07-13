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
	virtual void SetLatestModifiedTimeStamp(time_t ts); //设置修改时间	

	//void SetSymlinkNode(NodeType link_type, BaseNode* target_node, string_local target_name = nullptr); //设置链接对象
	//BaseNode* GetSymlinkNode() const { return m_target_node; } //获取链接对象	
	//string_local GetSymlinkName()const { return m_target_name; } //获取链接对象名

	void SetTarget(NodeType type, string_local path);
	string_local GetTargetPath() { return m_target_path_str; }
	
private:
	string_local m_target_path_str = TEXT("");
};

#endif // !__SYMLINKNODE_H__
