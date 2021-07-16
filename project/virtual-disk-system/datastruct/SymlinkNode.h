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
	virtual void SetLatestModifiedTimeStamp(time_t ts); //�����޸�ʱ��	

	//void SetSymlinkNode(NodeType link_type, BaseNode* target_node, string_local target_name = nullptr); //�������Ӷ���
	//BaseNode* GetSymlinkNode() const { return m_target_node; } //��ȡ���Ӷ���	
	//string_local GetSymlinkName()const { return m_target_name; } //��ȡ���Ӷ�����

	void SetTarget(NodeType type, string_local path);
	string_local GetTargetPath() { return m_target_path_str; }
	
private:
	string_local m_target_path_str = TEXT("");
};

#endif // !__SYMLINKNODE_H__
