//<���ӽڵ�>
//��Ա���ԣ����Ӷ���ָ�룩
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
	virtual const time_t GetLatestModifiedTimeStamp(); //��ȡ�޸�ʱ��

	void SetSymlinkNode(NodeType link_type, BaseNode* dst_node); //�������Ӷ���
	BaseNode* GetSymlinkNode() const { return m_target_node; } //��ȡ���Ӷ���
	string_local GetSymlinkName()const { return m_target_name; } //��ȡ���Ӷ�����

private:
	BaseNode* m_target_node = nullptr;
	string_local m_target_name = TEXT("");
	virtual void SetLatestModifiedTimeStamp(time_t ts); //�����޸�ʱ��	
};

#endif // !__SYMLINKNODE_H__
