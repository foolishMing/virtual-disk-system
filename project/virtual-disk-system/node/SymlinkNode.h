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
	virtual const time_t GetLatestModifiedTimeStamp(); //��ȡ�޸�ʱ��

	void SetSymlinkNode(NodeType link_type, BaseNode* dst_node); //�������Ӷ���
	BaseNode* GetSymlinkNode() const { return m_dst_node; } //��ȡ���Ӷ���

private:
	BaseNode* m_dst_node = nullptr;
	virtual void SetLatestModifiedTimeStamp(time_t ts); //�����޸�ʱ��	
};

#endif // !__SYMLINKNODE_H__
