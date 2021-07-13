//<Ŀ¼�ڵ�>
#pragma once 
#ifndef  __DIRNODE_H__
#define __DIRNODE_H__

#include "BaseNode.h"

class DirNode : public BaseNode {
public:
	explicit DirNode(string_local name, BaseNode* parent = nullptr);
	virtual ~DirNode();

	virtual const size_t GetSize() const;
	virtual const time_t GetLatestModifiedTimeStamp(); //��ȡ�޸�ʱ��

	BaseNode* FindChildByName(const string_local& node_name); //���ݽڵ����ƻ�ȡ�ӽڵ�
	bool ContainsChild(const string_local& node_name); //���ݽڵ������ж��ӽڵ��Ƿ����
	void AppendChild(BaseNode* node); //��Children�б�ĩβ׷��(�ǿ�)�ӽڵ�
	bool RemoveChildByIndex(int index); //ɾ����index�����ӽڵ�
	std::vector<BaseNode*> Children() { return m_children; }

private:
	std::vector<BaseNode*> m_children = {};
	virtual void SetLatestModifiedTimeStamp(time_t ts); //�����޸�ʱ��	
};

#endif // ! __DIRNODE_H__