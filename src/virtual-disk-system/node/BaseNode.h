#pragma once
#ifndef __BASENODE_H__
#define __BASENODE_H__

#include "../util/Common.h"

//�ڵ�����
enum NodeType {
	Directory,		//Ŀ¼
	File,			//�ļ�
	SymlinkD,		//��ݷ�ʽ(Ŀ¼)
	SymlinkF		//��ݷ�ʽ(�ļ�)	
};

class BaseNode{
public:
	explicit BaseNode();
	explicit BaseNode(string_local& name, BaseNode* parent = nullptr);
	virtual ~BaseNode();
	
	NodeType GetType();

	string_local GetName();
	void SetName(const string_local& name);

	virtual uint64_t GetSize() = 0;
	
	BaseNode* GetParent();	//��ȡ���ڵ�ָ��
	void SetParent(BaseNode* parent);	//���ø��ڵ�ָ��

	bool IsNameEqualsTo(const string_local& name);	//�жϸ��ַ����Ƿ���ڵ�������ͬ
protected:
	uint64_t size;						
	string_local m_name;
	NodeType m_type;
	BaseNode* m_parent = nullptr;
};

#endif // !__BASENODE_H__
