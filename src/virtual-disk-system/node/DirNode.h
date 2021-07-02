#pragma once 
#ifndef  __DIRNODE_H__
#define __DIRNODE_H__

#include "BaseNode.h"

class DirNode : public BaseNode {
public:
	explicit DirNode(string_local name, BaseNode* parent = nullptr);
	virtual ~DirNode();
	BaseNode* FindChildByName(const string_local& node_name);	//���ݽڵ����ƻ�ȡ�ӽڵ�
	bool ContainsChild(const string_local& node_name);	//���ݽڵ������ж��ӽڵ��Ƿ����
	void DeleteChildByName(const string_local& node_name);	//���ݽڵ�����ɾ���ӽڵ�
	void DisposeChildByName(const string_local& node_name);	//���ݽڵ������Ƴ��ýڵ�(��ɾ��)
	void AppendChild(BaseNode* node);	//��Children�б�ĩβ׷��(�ǿ�)�ӽڵ�
	virtual uint64_t GetSize();

private:
	std::vector<BaseNode*> m_children;
};

#endif // ! __DIRNODE_H__